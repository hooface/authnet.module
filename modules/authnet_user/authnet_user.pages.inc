<?php

/**
 * @file
 * Authorize.net - User Pages
 */

/**
 * User Billing page
 */
function authnet_user_billing_page($user) {

  drupal_set_title('Saved cards');

  // Start output.
  $output = '';

  // Create a link to add a new profile.
  $output .= l('+ Add a credit card', 'user/' . $user->uid . '/billing/add');

  // Load the customer profile from Authorize.net.
  $customer_profile = authnet_cim_entity_profile_load('user', $user->uid);

  // Display an empty text message if no profiles are available.
  if (empty($customer_profile) || empty($customer_profile->paymentProfiles)) {
    $output .= t('You don\'t have any saved cards. Click "Add a credit card" above to save a new one.');
  }

  // Load the list of payment profiles.
  $profiles_list = authnet_ui_payment_profile_options($customer_profile);

  // Put together header and rows for the payment profile table.
  $header = array(
    t('Credit card'),
    t('Operations'),
  );
  $rows = array();
  foreach ($profiles_list as $payment_profile) {
    $rows[] = array(
      l($payment_profile, 'user/' . $user->uid . '/billing/edit'),
      l('Edit', 'user/' . $user->uid . '/billing/edit') . ' | ' . l('Delete', 'user/' . $user->uid . '/billing/delete'),
    );
  }

  // Add the themed table to the output.
  $output .= theme('table', $header, $rows);

  // Return the output.
  return $output;
}

/**
 * User payment profile form page callback.
 *
 * @param $user
 *   The user account object.
 * @param $pid
 *   The id of the payment profile database record (optional).
 */
function authnet_user_payment_profile($user, $pid = NULL) {

  // Set the page title.
  if ($pid) {
    drupal_set_title(t('Edit payment profile'));
  }
  else {
    drupal_set_title(t('Add a payment profile'));
  }

  // Load the user's customer profile id.
  $customer_profile_id = authnet_cim_entity_profile_id_load('user', $user->uid);

  // Load the payment profile id.
  $payment_profile_id = NULL;
  if ($pid) {
    $payment_profile_id = authnet_cim_entity_payment_profile_id_load($pid);
  }

  // Return the payment profile form.
  return drupal_get_form('authnet_ui_payment_profile_form', $customer_profile_id, $payment_profile_id);
}