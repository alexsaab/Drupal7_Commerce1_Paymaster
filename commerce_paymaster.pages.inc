<?php
/**
 * @file
 * Page callbacks for Commerce PayMaster.
 */

/**
 * Payment result callback.
 */
function commerce_paymaster_result_page_callback()
{
	if (!empty($_POST))
	{

		watchdog('paymaster', print_r($_POST, true));
		if (isset($_POST["LMI_PREREQUEST"]) && ($_POST["LMI_PREREQUEST"] == "1" || $_POST["LMI_PREREQUEST"] == "2"))
		{
			print 'YES';
			die;
		}

		if ($order = commerce_order_load($_POST['LMI_PAYMENT_NO']))
		{
			$payment_method = commerce_payment_method_instance_load($order->data['payment_method']);
			$secret_key     = $payment_method['settings']['paymaster_secret'];
			$sign_method    = $payment_method['settings']['paymaster_sign_method'];

			$hash = commerce_paymaster_get_hash($_POST["LMI_MERCHANT_ID"], $_POST["LMI_PAYMENT_NO"], $_POST["LMI_SYS_PAYMENT_ID"], $_POST["LMI_SYS_PAYMENT_DATE"], $_POST["LMI_PAYMENT_AMOUNT"], $_POST["LMI_CURRENCY"], $_POST["LMI_PAID_AMOUNT"], $_POST["LMI_PAID_CURRENCY"], $_POST["LMI_PAYMENT_SYSTEM"], $_POST["LMI_SIM_MODE"], $secret_key, $sign_method);

			$sign = commerce_paymaster_get_sign($_POST["LMI_MERCHANT_ID"], $_POST['LMI_PAYMENT_NO'], number_format((float) $_POST["LMI_PAYMENT_AMOUNT"], 2, '.', ''), $_POST["LMI_CURRENCY"], $secret_key, $sign_method);

			if ($hash == $_POST['LMI_HASH'] && $_POST["SIGN"] == $sign)
			{
				$order_wrapper = entity_metadata_wrapper('commerce_order', $order);

				commerce_order_status_update($order, variable_get('paymaster_order_status', 'pending'), false, NULL, t('Paymaster payment was processed!'));

				commerce_paymaster_create_transaction($order, $order_wrapper, $_POST['LMI_SYS_PAYMENT_ID']);

				print 'OK';
			}
			else
			{
				print 'WRONG HASH';
				die;
			}
		}
		else
		{
			print 'NO ORDER';
			die;
		}

	}
	else
	{
		print 'NO POST';
		die;
	}
}

/**
 * Payment fail callback.
 */
function commerce_paymaster_fail_page_callback()
{
	drupal_set_message(t('Payment unsuccessful!'), 'error');

	if (!empty($_POST['LMI_PAYMENT_NO']))
	{
		if ($order = commerce_order_load($_POST['LMI_PAYMENT_NO']))
		{
			commerce_payment_redirect_pane_previous_page($order);
			drupal_goto(commerce_checkout_order_uri($order));
		}
	}

	drupal_goto('<front>');
}

/**
 * Payment success callback.
 */
function commerce_paymaster_success_page_callback()
{
	$keys = array(
		'LMI_PAYMENT_NO',
		'LMI_SYS_PAYMENT_ID'
	);

	$has_error = false;
	foreach ($keys as $key)
	{
		if (empty($_POST[$key]))
		{
			$has_error = true;
			break;
		}
	}

	if (!$has_error)
	{
		if ($order = commerce_order_load($_REQUEST['LMI_PAYMENT_NO']))
		{
			drupal_set_message(t('Thank you, payment successful.'));
			commerce_payment_redirect_pane_next_page($order);
			drupal_goto(commerce_checkout_order_uri($order));
		}
	}

	drupal_set_message(t('Unknown error.'));
	drupal_goto('<front>');
}