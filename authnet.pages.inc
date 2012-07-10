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

  // AIM sandbox mode toggle
  $form['authnet_aim_sandbox'] = array(
    '#type' => 'radios',
    '#title' => t('AIM sandbox'),
    '#description' => t('If sandbox mode is turned on, AIM transactions will be posted to the Authorize.Net test server. Otherwise, they will be posted directly to the live Authorize.Net gateway'),
    '#options' => array(
      t('Off'),
      t('On'),
    ),
    '#default_value' => variable_get('authnet_aim_sandbox', 0),
  );

  // CIM validation mode toggle
  $form['authnet_cim_validation_mode'] = array(
    '#type' => 'radios',
    '#title' => t('CIM validation mode'),
    '#description' => t('Validation mode allows you to generate a test transaction at the time you create a customer profile. In Test Mode, only field validation is performed. In Live Mode, a transaction is generated and submitted to the processor with the amount of $0.00 or $0.01. If successful, the transaction is immediately voided.'),
    '#options' => array(
      'none' => t('Off'),
      'testMode' => t('Test Mode'),
      'liveMode' => t('Live Mode'),
    ),
    '#default_value' => variable_get('authnet_cim_validation_mode', 'none'),
  );

  return system_settings_form($form);
}