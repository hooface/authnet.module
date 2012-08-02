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

  // Display an empty text message if no profiles are available.
  if (empty($customer_profile) || empty($customer_profile->paymentProfiles)) {
    $output .= t('You don\'t have any saved cards. Click "Add a credit card" above to save a new one.');
  }

  // Return the output.
  return $output;
}

/**
 * User payment profile form.
 *
 * @param $user
 *   The user account object.
 * @param $payment_profile_id
 *   The id of the payment profile to edit.
 */
function authnet_user_payment_profile_form(&$form_state, $user, $payment_profile_id = NULL) {

  // Set the page title.
  if ($payment_profile_id) {
    drupal_set_title(t('Edit payment profile'));
  }
  else {
    drupal_set_title(t('Add a payment profile'));
  }

  // Load the user's customer profile id.
  $customer_profile_id = authnet_cim_entity_profile_id_load('user', $user->uid);

  // Start with an empty form.
  $form = array();

  /**
   * Save the user id value to the form. This will be used to generate information for the Authorize.net 
   * customer profile, if one doesn't exist already.
   * @see authnet_user_authnet_ui_form_customer_profile_new()
   */
  $form['uid'] = array(
    '#type' => 'value',
    '#value' => $user->uid,
  );

  // Add the Authnet UI payment profile form.
  $form += authnet_ui_payment_profile_form($form_state, $customer_profile_id, $payment_profile_id);

  // Redirect to the user's billing list.
  $form['#redirect'] = 'user/' . $user->uid . '/billing';

  // Return the payment profile form.
  return $form;
}

/**
 * User payment profile delete form.
 *
 * @param $user
 *   The user account object.
 * @param $pid
 *   The id of the payment profile to delete.
 */
function authnet_user_payment_profile_delete_form(&$form_state, $user, $payment_profile_id) {

  // Start an empty form array.
  $form = array();

  // Store the payment profile id value in the form.
  $form['payment_profile_id'] = array(
    '#type' => 'value',
    '#value' => $payment_profile_id,
  );

  // Redirect to the user's billing list in the end.
  $redirect = 'user/' . $user->uid . '/billing';
  $form['#redirect'] = $redirect;

  // Return a confirmation form.
  return confirm_form($form, 'Are you sure you want to delete this payment profile?', $redirect);
}

/**
 * User payment profile delete form submit.
 */
function authnet_user_payment_profile_delete_form_submit($form, &$form_state) {

  // Load the payment profile record.
  $payment_profile = authnet_cim_entity_payment_profile_load($form_state['values']['payment_profile_id']);

  // Delete the payment profile.
  authnet_cim_payment_profile_delete($payment_profile['customer_profile_id'], $payment_profile['payment_profile_id']);
}