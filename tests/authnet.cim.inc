<?php

/**
 * @file
 * Authorize.net - CIM test code
 */

/**
 * The following are some sample PHP snippets for testing the API wrapper functions.
 * Just paste this into the Devel module's /devel/php to test it out.
 * It may not be SimpleTest, but it's better than nothing.
 */

?>

<?php

/**
 * Create a new customer profile based on a user
 */
$uid = 1;
$user = user_load($uid);

$values = array(
  'merchantCustomerId' => $user->uid,
  'description' => $user->name,
  'email' => $user->mail,
  'paymentProfiles' => array(
    array(
      'customerType' => 'individual',
      'billTo' => array(
        'firstName' => 'John',
        'lastName' => 'Doe',
        'company' => 'Company, Inc.',
        'address' => '123 Fake Street',
        'city' => 'Pleasantville',
        'state' => 'CA',
        'zip' => '123456',
        'country' => 'USA',
        'phoneNumber' => '123 456 7890',
        'faxNumber' => '789 123 4567',
      ),
      'payment' => array(
        'creditCard' => array(
          'cardNumber' => '4007000000027',
          'expirationDate' => '2012-12',
          'cardCode' => '123',
        ),
      ),
    ),
  ),
);
$cim_profile = authnet_customer_profile_new($values);
dpm($cim_profile);
authnet_cim_profile_save($cim_profile);
dpm($cim_profile);

?>

<?php

/**
 * Load an existing customer profile based on a customerProfileId.
 */

$customerProfileId = XXXXXXXX;

$cim_profile = authnet_cim_profile_load($customerProfileId);

dpm($cim_profile);

?>

<?php

/**
 * Update an existing profile.
 */

$customerProfileId = XXXXXXXX;
$cim_profile = authnet_cim_profile_load($customerProfileId);

dpm($cim_profile);

$cim_profile->description = 'TEST TEST TEST';

authnet_cim_profile_save($cim_profile);

dpm($cim_profile);

?>

<?php

/**
 * Delete a profile.
 */

$customerProfileId = XXXXXXXX;
authnet_cim_profile_delete($customerProfileId);

?>

<?php

/**
 * Load a payment profile.
 */

$customerProfileId = XXXXXXXX;
$customerPaymentProfileId = XXXXXXXX;

$payment_profile = authnet_cim_payment_profile_load($customerProfileId, $customerPaymentProfileId);

dpm($payment_profile);

?>

<?php

/**
 * Save a new payment profile.
 */

$customerProfileId = XXXXXXXX;

$values = array(
  'customerType' => 'individual',
  'billTo' => array(
    'firstName' => 'John',
    'lastName' => 'Doe',
    'company' => 'Company, Inc.',
    'address' => '123 Fake Street',
    'city' => 'Pleasantville',
    'state' => 'CA',
    'zip' => '123456',
    'country' => 'USA',
    'phoneNumber' => '123 456 7890',
    'faxNumber' => '789 123 4567',
  ),
  'payment' => array(
    'bankAccount' => array(
      'accountType' => 'checking',
      'routingNumber' => '123456789',
      'accountNumber' => '987654321',
      'nameOnAccount' => 'John Doe',
      'bankName' => 'Bank, Inc.',
    ),
  ),
);

$paymentProfile = authnet_payment_profile_new($values);

dpm($paymentProfile);

authnet_cim_payment_profile_save($customerProfileId, $paymentProfile);

dpm($paymentProfile);

?>

<?php

/**
 * Update an existing payment profile.
 */

$customerProfileId = XXXXXXXX;
$paymentProfileId = XXXXXXXX;

$paymentProfile = authnet_cim_payment_profile_load($customerProfileId, $paymentProfileId);

dpm($paymentProfile);

$paymentProfile->billTo->firstName = 'Mike';

authnet_cim_payment_profile_save($customerProfileId, $paymentProfile);

dpm($paymentProfile);

?>

<?php

/**
 * Delete an existing payment profile.
 */

$customerProfileId = XXXXXXXX;
$paymentProfileId = XXXXXXXX;

$paymentProfile = authnet_cim_payment_profile_delete($customerProfileId, $paymentProfileId);

?>

<?php

/**
 * Load a shipping address.
 */

$customerProfileId = XXXXXXXX;
$customerShippingAddressId = XXXXXXXX;

$address = authnet_cim_shipping_address_load($customerProfileId, $customerShippingAddressId);

dpm($address);

?>

<?php

/**
 * Save a new shipping address.
 */

$customerProfileId = XXXXXXXX;

$values = array(
  'firstName' => 'John',
  'lastName' => 'Doe',
  'company' => 'Company, Inc.',
  'address' => '123 Fake Street',
  'city' => 'Pleasantville',
  'state' => 'CA',
  'zip' => '123456',
  'country' => 'USA',
  'phoneNumber' => '123 456 7890',
  'faxNumber' => '789 123 4567',
);

$address = authnet_address_new($values);

dpm($address);

authnet_cim_shipping_address_save($customerProfileId, $address);

dpm($address);

?>

<?php

/**
 * Update an existing shipping address.
 */

$customerProfileId = XXXXXXXX;
$customerAddressId = XXXXXXXX;

$address = authnet_cim_shipping_address_load($customerProfileId, $customerAddressId);

dpm($address);

$address->firstName = 'Mike';

authnet_cim_shipping_address_save($customerProfileId, $address);

dpm($address);

?>

<?php

/**
 * Delete a shipping address.
 */

$customerProfileId = XXXXXXXX;
$customerAddressId = XXXXXXXX;

authnet_cim_shipping_address_delete($customerProfileId, $customerAddressId);

?>

<?php

/**
 * Create a new transaction.
 */

$customerProfileId = XXXXXXXX;
$customerPaymentProfileId = XXXXXXXX;

$values = array(
  'amount' => '100.00',
  'customerProfileId' => $customerProfileId,
  'customerPaymentProfileId' => $customerPaymentProfileId,
);

$transaction = authnet_transaction_new($values);

dpm($transaction);

$transaction_id = authnet_cim_transaction($transaction, AUTHNET_METHOD_AUTH_ONLY);

dpm($transaction_id);

?>