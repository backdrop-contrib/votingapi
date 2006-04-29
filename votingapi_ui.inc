<?php
/* $Id$ */


/*
 * Basic VotingAPI settings, mostly cron related
 */

function votingapi_settings_page() {
  $form['#tree'] = TRUE;

  $form['votingapi_calculation_schedule'] = array(
    '#type' => 'radios',
    '#title' => t('Vote tallying'),
    '#description' => t('On high-traffic sites, administrators can use this setting to postpone the calculation of vote results.'),
    '#default_value' => variable_get('votingapi_calculation_schedule', 'immediate'),
    '#options' => array(
      'immediate' => t('Tally results whenever a vote is cast'),
      'cron' => t('Tally results at cron-time'),
      'manual' => t('Never tally votes: I am using a custom module to control vote results')
    ),
  );
/*
  $form['actions'] = array(
    '#type' => 'radios',
    '#title' => t('Actions'),
    '#description' => t('Voting actions can perform tasks depending on the results of a vote. Configure them on the voting actions page.'),
    '#default_value' => 1,
    '#options' => array(
      1 => t('Allow modules to register their own voting actions'),
      2 => t('Only perform voting actions I configure'),
      3 => t('Disable voting actions ')
    ),
  );
*/

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit settings')
  );

  $output .= drupal_get_form('votingapi_settings_form', $form);
  return $output;
}

function votingapi_settings_form_submit($form_id, $form_values) {
  drupal_set_message(t('VotingAPI settings have been saved.'));
}


/*
 * A peek at the raw voting data. Mostly useful for administrators
 * who need to poke around, but don't want to wade through dba.module's
 * full output.
 */
 
function votingapi_vote_data_page() {
  $output .= '<h3>' . t('VotingAPI data') . '</h3>';
  $output .= '<p>' . t('Coming soon.') . '</p>';

  $form['#tree'] = TRUE;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit changes')
  );

  $output .= drupal_get_form('votingapi_vote_data_form', $form);
  return $output;
}

function votingapi_vote_data_form_submit($form_id, $form_values) {

}
 
/*
 * Complicated stuff here! These pages allow users to maintain existing
 * and create new action sets.
 */

function votingapi_actions_page() {
  $output .= '<h3>' . 'VotingAPI actions' . '</h3>';
  $output .= '<p>' . 'Votes can be tallied for each piece of content. When the results are calculated, they can be used to launch other actions (like promoting a node to the front page, or notifying the administrator of objectionable content). Use these Voting Action sets to control these behaviors.' . '</p>';
  $output .= '<p>' . l('Click here', 'admin/voting/actions/edit') . ' to create a new action set from scratch.' . '</p>';
  
  $db_sets = _votingapi_load_action_sets_from_db();
  $module_sets = _votingapi_load_action_sets_from_modules();

  $header = array(t('name'), t('content-type'), t('status'), t('operations'));
  $rows = array();
  foreach ($db_sets as $key=>$set) {
    $row = array();
    $row[] = $set['name'];
    $row[] = $set['content_type'];
    $row[] = $set['source'];
    $row[] = $set['status'] ? 'disabled' : 'enabled';
    $row[] = l('edit', 'admin/voting/actions') . ' | ' . l('disable', 'admin/voting/actions') . ' | ' . l('delete', 'admin/voting/actions');
    $rows[] = $row;
  }
  $output .= '<h4>' . t('User-configured sets') . '</h4>';
  $output .= theme('table', $header, $rows);


  $header = array(t('name'), t('content-type'), t('source'), t('status'), t('operations'));
  $rows = array();
  foreach ($module_sets as $key=>$set) {
    $row = array();
    $row[] = $set['name'];
    $row[] = $set['content_type'];
    $row[] = $set['source'];
    $row[] = $set['status'] ? 'disabled' : 'enabled';
    $row[] = l('override', 'admin/voting/actions/clone/' . $key) . ' | ' . l('disable', 'admin/voting/actions/disable/' . $key);
    $rows[] = $row;
  }
  $output .= '<h4>' . t('Module-supplied sets') . '</h4>';
  $output .= theme('table', $header, $rows);

  return $output;
}


function votingapi_actions_edit_page($vasid = NULL) {
  $form['#tree'] = TRUE;

  $form['main_set'] = _votingapi_get_set_form($vasid);

/*
   mask varchar(8) default 'AND', * affects criteria
*/

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes')
  );

  $output .= drupal_get_form('votingapi_actions_edit_form', $form);
  return $output;
}

function votingapi_actions_edit_form_submit($form_id, $form_values) {

}

function _votingapi_get_set_form($set = NULL) {
  $content_types = array('' => '- Any content type -');
  $result = db_query('SELECT DISTINCT type FROM {actions}');
  while ($content_type = db_fetch_object($result)) {
    $content_types[$content_type->type] = $content_type->type;
  }

  $form['vasid'] = array(
    '#type' => 'hidden',
    '#value' => $set->vasid,
  );

  $form['parent'] = array(
    '#type' => 'hidden',
    '#value' => $set->parent,
  );

  $form['source'] = array(
    '#type' => 'hidden',
    '#value' => $set->parent ? 'User-created' : $set->parent,
  );

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => 'Set name',
    '#default_value' => $set->name,
    '#size' => 32,
    '#maxlength' => 128,
  );

  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => 'Set description',
    '#default_value' => $set->description,
    '#size' => 64,
    '#maxlength' => 255,
  );

  $form['content_type'] = array(
    '#type' => 'select',
    '#default_value' => $set->content_type,
    '#options' => $content_types,
    '#title' => 'Content type',
    '#description' => 'Determines the content type this action set applies to. Node and Comment are the most common.',
  );

  $form['required'] = array(
    '#type' => 'checkbox',
    '#default_value' => $set->required,
    '#title' => 'Set is required',
    '#description' => 'In complex hierarchial action sets, sub-sets marked as required will abort the entire action set if their criteria are not met.',
  );

  $form['criteria'] = _votingapi_get_criteria_form($set);
  $form['actions'] = _votingapi_get_actions_form($set);
  
  return $form;
}

function _votingapi_get_criteria_form($set) {
  $form = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Action critiera'),
  );
  return $form;
}

function _votingapi_get_actions_form($set) {
  $form = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Actions'),
  );
  return $form;
}

/*
 * Deleting a given voting action.
 */

function votingapi_actions_delete_page($vasid) {
  $form['vasid'] = array(
    '#type' => 'hidden',
    '#value' => $vasid,
  );

  $output = confirm_form(
    'votingapi_actions_delete_confirm',
    $form,
    t('Are you sure you want to delete this voting action set?'),
    $_GET['destination'] ? $_GET['destination'] : 'admin/voting',
    t('This action cannot be undone.'),
    t('Delete Set'),
    t('Cancel') 
  );
  return $output;
}

function votingapi_actions_delete_confirm_submit($form_id, $form_values) {
  if ($form_values['confirm']) {
    if (user_access('administer votingapi')) {
      $vasid = $form_values['vasid'];

      
      drupal_set_message(t('Set deleted.'));
      return('admin/voting');
    }
    else {
      drupal_access_denied();
    }
  }
}