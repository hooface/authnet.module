<?php

/**
 * @file
 * Authorize.net API Pages
 */

/**
 * Settings form.
 */
function authnet_settings_form() {

  // Authorize.net Login ID and Transaction Key
  $form['authnet_login_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Login ID'),
    '#default_value' => variable_get('authnet_login_id', ''),
    '#required' => TRUE,
  );
  $form['authnet_transaction_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Transaction Key'),
    '#default_value' => variable_get('authnet_transaction_key', ''),
    '#required' => TRUE,
  );

  return system_settings_form($form);
}