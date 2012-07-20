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

  // Theme the list as an item list.
  $output .= theme('item_list', $profiles_list);

  // Return the output.
  return $output;
}
