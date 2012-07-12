<?php

/**
 * @file
 * Authorize.net - Ubercart Integration - Charge
 *
 * Helpers for the payment gateway charge callback
 */


/**
 * Helper to authnet_uc_charge().
 * (Callback for payment gateway settings)
 */
function _authnet_uc_charge($order_id, $amount, $data) {
  // Load the order.
  $order = uc_order_load($order_id);
  
  // Perform the appropriate action based on the transaction type.
  switch ($data['txn_type']) {
    // Reference transactions are handled through Authorize.Net's CIM.
    case UC_CREDIT_REFERENCE_TXN:
      return _authnet_uc_cim_profile_charge($order, $amount, $data);

    // Set a reference only.
    case UC_CREDIT_REFERENCE_SET:
      // Return the error message if this failed.
      if ($message = _authnet_uc_cim_profile_create($order)) {
        return array('success' => FALSE, 'message' => $message);
      }
      else {
        return array('success' => TRUE, 'message' => t('New customer profile created successfully at Authorize.Net.'));
      }

    // Accommodate all other transaction types.
    default:
      return _authnet_uc_charge($order, $amount, $data);
  }
}


/**
 * Create a CIM profile using an order's data.
 */
function _authnet_uc_cim_profile_create($order){
  $cim_profile = _authnet_uc_cim_profile_from_order($order);
  $success = authnet_cim_profile_save($cim_profile);

  // Fail
  if(!$success){
    $message = authnet_get_error_message();
    $comment = t('Authorize.Net: Creating CIM profile failed.<br />!message',array('!message' => $message));
    uc_order_comment_save($order->order_id, 0, $comment, 'admin');
    return $message;
  }

  // Success
  $comment = t('Authorize.Net: CIM profile created - @id', array('@id' => $cim_profile->customerProfileId));
  uc_order_comment_save($order->order_id, 0, $comment);

  // Save the new credit reference to the db.
  $order->data = uc_credit_log_reference($order->order_id, $cim_profile->customerProfileId, $order->payment_details['cc_number']);

  return '';
}

/**
 * Use a reference to charge to a CIM profile.
 */
function _authnet_uc_cim_profile_charge($order, $amount, $data){
  return array('success' => FALSE, 'message' => t('Not yet implemented'));
}

/**
 * Handles authorizations and captures through AIM at Authorize.Net
 *
 * @TODO: Make this function actually work
 */
function _authnet_uc_charge($order, $amount, $data){
  return array('success' => FALSE, 'message' => t('Not yet implemented'));
}


/* ******************************************************************************
 * Helpers
 * *****************************************************************************/

/**
 * Create a CIM profile using an order's data.
 */
function _authnet_uc_cim_profile_from_order($order){
  // @todo: uc_authorizenet sets a refId. Should we??
  // $refId = substr($order->order_id .'-'. time(), 0, 20);

  // @todo: entity based customer ID
  //$merchantCustomerId = !empty($order->uid) ? 'user:'.$order->uid : 'order:'.$order->order_id;
  $merchantCustomerId = $order->uid;

  // @todo: What's the deal with all these substrs? should we move that logic to the api??
  $values = array(
    'merchantCustomerId' => substr($merchantCustomerId, 0, 20),
    'description' => substr(t('Order @order taking place at @date', array('@order' => $order->order_id, '@date' => format_date(time()))), 0, 255),
    'email' => substr($order->primary_email, 0, 255),
    'paymentProfiles' => array(_authnet_uc_payment_profile_from_order($order));
    'shipToList' => array(_authnet_uc_address_from_order($order, $type = 'delivery'));
  );
  return authnet_customer_profile_new($values);
}

/**
 * Get address info from order
 */
function _authnet_uc_payment_profile_from_order($order){
  return array(
    'customerType' => 'individual',
    'billTo' => _authnet_uc_address_from_order($order, 'billing');
    'payment' => _authnet_uc_payment_from_order($order);
  );
}

/**
 * Get address info from order
 */
function _authnet_uc_address_from_order($order, $type = 'billing') {
  $country = uc_get_country_data(array('country_id' => $order->{$type.'_country'}));
  return array(
    'firstName' => substr($order->{$type.'_first_name'}, 0, 50),
    'lastName' => substr($order->{$type.'_last_name'}, 0, 50),
    'company' => substr($order->{$type.'_company'}, 0, 50),
    'address' => substr($order->{$type.'_street1'}, 0, 60),
    'city' => substr($order->{$type.'_city'}, 0, 40),
    'state' => substr(uc_get_zone_code($order->{$type.'_zone'}), 0, 2),
    'zip' => substr($order->{$type.'_postal_code'}, 0, 20),
    'country' => !$country ? '' : $country[0]['country_iso_code_2'],
  );
}

/**
 * Get payment info from order
 */
function _authnet_uc_payment_from_order($order) {
  return array(
    'creditCard' => array(
      'cardNumber' => $order->payment_details['cc_number'],
      'expirationDate' => $order->payment_details['cc_exp_year'] .'-'. str_pad($order->payment_details['cc_exp_month'], 2, '0', STR_PAD_LEFT),
    ),
  );
}