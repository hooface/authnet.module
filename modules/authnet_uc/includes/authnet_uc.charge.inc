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
  return t('Not yet Implemented');
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

