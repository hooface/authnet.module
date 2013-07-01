<?php

/**
 * @file
 * Support for Rules module for authnet_arb_entity.
 */

/**
 * Implements hook_rules_event_info().
 * @ingroup rules
 */
function authnet_arb_entity_rules_event_info() {
  return array(
    'authnet_arb_entity_rule_silentpost' => array(
      'label' => t("User's Authorize.net ARB Silent Post is received"),
      'group' => t('User'),
      'variables' => array(
        'account' => array('type' => 'user', 'label' => t('User associated to the ARB')),
        'response_code' => array('type' => 'integer', 'label' => t('Response code of the ARB transaction')),
        'post' => array('type' => 'list<token>', 'label' => t('ARB Post data')),
      ),
    ),
  );
}

/**
 * Implements hook_rules_condition_info().
 */
function authnet_arb_entity_rules_condition_info() {
  return array(
    'authnet_arb_entity_rules_condition_silentpost' => array(
      'label' => t('ARB Silent Post Received'),
      'parameter' => array(
        'expected' => array(
          'type' => 'integer',
          'label' => t('ARB Silent Post returns'),
          'options list' => 'authnet_arb_entity_response_options',
          'description' => t('Select which response to act against.'),
          'restriction' => 'input',
        ),
      ),
      'group' => t('User'),
    ),
  );
}

/**
 * Callback for Rules condition.
 */
function authnet_arb_entity_rules_condition_silentpost($expected, $state, $element) {
  return $expected == $element->variables['response_code'];
}
