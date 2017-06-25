# drupal7_commerce_paymaster

UC Paymaster module for Drupal 7 and Commerce

Модуль предназначен для работы с Drupal 7 и Commerce (1)

1. Залить через ftp в папку modules.
2. Включить модуль "Commerce Paymaster" В директории сайта admin/modules
3. В директории сайта admin/commerce/config/payment-methods/manage/commerce_payment_commerce_paymaster произвести настройки:
Merchant id - идентификатор сайта (его можно взять в личном кабинете PayMaster), 
Merchant secret key - секретный ключ (изначально задается в личном кабинете PayMaster).
Currency - ISO код валюты, с которой работает мерчант (например RUB или USD)
4. В личном кабинете PayMaster (Список сайтов->Настройки->Обратные вызовы):
В Payment notification выбрать POST-запрос и прописать: http://ВАШ_САЙТ.ru/cart/paymaster/result
В Success redirect выбрать POST-запрос и прописать: http://ВАШ_САЙТ.ru/cart/paymaster/success
В Failure redirect выбрать POST-запрос и прописать: http://ВАШ_САЙТ.ru/cart/paymaster/fail
 

_Все вопросы по разработке/доработке модуля присылайте на awa77 собака mail.ru
Автор Алексей А._ 