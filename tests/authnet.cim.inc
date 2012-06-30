<?php

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

// Optionally load a description.
$description = '';
if (function_exists('content_profile_load')) {
  $profile = content_profile_load('customer', $uid);
  $description = $profile->title;
}

$values = array(
  'merchantCustomerId' => $user->uid,
  'description' => $description,
  'email' => $user->mail
);
$cim_profile = authnet_cim_profile_new($values);
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