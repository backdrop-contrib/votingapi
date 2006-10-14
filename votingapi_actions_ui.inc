<?php
// $Id$

// Contains UI management functions for the votingapi_actions module.

function votingapi_actions_admin_page() {
  $numSets = 25;

  votingapi_rebuild_action_cache();

  drupal_set_title(t('administer voting actions'));

  foreach (_votingapi_load_action_sets_from_db() as $name => $set) {
    $links = array();
    $links['votingapi_actions_edit'] = array(
       'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/gtk-edit.png'),
       'html' => TRUE,
       'href' => "admin/voting/actions/edit/$name",
       'attributes' => array('title' => t('Edit this action set')),
    );
    $links['votingapi_actions_export'] = array(
       'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/document-save.png'),
       'html' => TRUE,
       'href' => "admin/voting/actions/export/$name",
       'attributes' => array('title' => t('Export this action set')),
    );
    $links['votingapi_actions_delete'] = array(
       'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/edit-delete.png'),
       'html' => TRUE,
       'href' => "admin/voting/actions/delete/$name",
       'attributes' => array('title' => t('Delete this action set')),
    );
    $items[] = array($name, $set['description'], theme('links', $links));
    $used[$name] = true;
  }

  if ($items) {
    $output = theme('table', array(t('Name'), t('Description'), t('Actions')), $items, array("cellpadding" => "4"), t('User-Defined Action Sets'));
    $output .= theme('pager', NULL, $numSets);
  }
  else {
    $output .= '<p>' . t('No action sets have currently been defined.') . '</p>';
  }

  $output .= t('<p>Below are system default action sets; if you clone one of these, or create another set with the same name, it will override the default.</p>');

  $items = array();
  $default_actions = _votingapi_load_action_sets_from_modules();
  $set_status = variable_get('votingapi_action_status', array());

  foreach ($default_actions as $name => $set) {
    if ($used[$name]) {
      $status = t('Overridden');
    }
    else if (isset($set_status[$name])) {
      if ($set_status[$name]) {
        $status = 'Enabled';
      }
      else {
        $status = 'Disabled';
      }
    }
    else if ($set['enabled']) {
      $status = 'Enabled';
    }
    else {
      $status = 'Disabled';
    }

    if ($status == 'Enabled') {
      $links = array();
      $links['votingapi_actions_add'] = array(
         'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/edit-paste.png'),
         'html' => TRUE,
         'href' => "admin/voting/actions/add/$name",
         'attributes' => array('title' => t('Clone this action set')),
      );
      $links['votingapi_actions_export'] = array(
         'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/document-save.png'),
         'html' => TRUE,
         'href' => "admin/voting/actions/export/$name",
         'attributes' => array('title' => t('Export this action set')),
      );
      $links['votingapi_actions_disable'] = array(
         'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/process-stop.png'),
         'html' => TRUE,
         'href' => "admin/voting/actions/disable/$name",
         'attributes' => array('title' => t('Disable this action set')),
      );
    }
    else if ($status == 'Disabled') {
      $links = array();
      $links['votingapi_actions_add'] = array(
         'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/edit-paste.png'),
         'html' => TRUE,
         'href' => "admin/voting/actions/add/$name",
         'attributes' => array('title' => t('Clone this action set')),
      );
      $links['votingapi_actions_export'] = array(
         'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/document-save.png'),
         'html' => TRUE,
         'href' => "admin/voting/actions/export/$name",
         'attributes' => array('title' => t('Export this action set')),
      );
      $links['votingapi_actions_enable'] = array(
         'title' => theme('image', drupal_get_path('module', 'votingapi_actions') . '/icons/media-playback-start.png'),
         'html' => TRUE,
         'href' => "admin/voting/actions/enable/$name",
         'attributes' => array('title' => t('Enable this action set')),
      );
    }
    else if ($status == 'Overridden') {
      $links = array();
    }

    $items[] = array($name, $set['source'], $set['description'], $status, theme('links', $links));
  }

  if ($items) {
    $output .= theme('table', array(t('Name'), t('Source'), t('Description'), t('Status'), t('Actions')), $items, array("cellpadding" => "4"), t('Default Action Sets'));
  }
  else {
    $output .= t('<p>No action sets have currently been defined.</p>');
  }

  return $output;
}

function votingapi_actions_admin_import_page($set_name = NULL) {
  $form['set'] = array(
    '#type' => 'textarea',
    '#title' => t('Import action set'),
    '#rows' => 20
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t("Submit"),
  );

  return $form;
}

/*
 * Handle the submit button on importing an action set.
 */
function votingapi_actions_admin_import_page_validate($form_id, &$form) {
  ob_start();
  eval($form['set']);
  ob_end_clean();

  if (is_array($sets)) {
    foreach($sets as $name => $set) {
      if ($name) {
        $errors = _votingapi_validate_action_set($name, $set);
        foreach($errors as $error) {
          form_set_error('', $error);
        }
      }
    }
  }
}

/*
 * Handle the submit button on importing an action set.
 */
function votingapi_actions_admin_import_page_submit($formid, $form) {
  ob_start();
  eval($form['set']);
  ob_end_clean();

  if (is_array($sets)) {
    foreach($sets as $name => $set) {
      if ($name) {
        $errors = _votingapi_validate_action_set($name, $set);
        if (count($errors) == 0) {
          _votingapi_insert_set($name, $set);
          votingapi_rebuild_action_cache();
          drupal_goto('admin/voting/actions');
        }
        else {
          foreach ($errors as $error) {
            drupal_set_message($error);
          }
        }
      }
      else {
        drupal_set_message(t('Unable to get an action set out of that.'));
        return;
      }
    }
  }
}

function votingapi_actions_admin_export_page($set_name) {
  $code = _votingapi_create_set_code($set_name);
  $lines = substr_count($code, "\n");
  $form['code'] = array(
    '#type' => 'textarea',
    '#title' => $set['name'],
    '#default_value' => $code,
    '#rows' => max($lines, 20)
  );
  return $form;
}

function votingapi_actions_admin_delete_page($set_name) {
  $form['set'] = array('#type' => 'value', '#value' => $set_name);
  return confirm_form(
    $form, 
    t('Are you sure you want to delete %title?', array('%title' => $set_name)), 
    $_GET['destination'] ? $_GET['destination'] : 'admin/voting/actions', 
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')  
  );
}

/*
 * Handle the submit button to delete a view.
 */
function votingapi_actions_admin_delete_page_submit($formid, $form) {
  $name = $form['set'];
  $sets = _votingapi_load_action_sets_from_db();
  if ($sets[$name]) {
    _votingapi_delete_set($name, $set[$name]);
  }
  return 'admin/voting/actions';
}

function votingapi_actions_admin_enable_page($set_name) {
  _votingapi_set_action_status($set_name, true);
  drupal_goto('admin/voting/actions');
}

function votingapi_actions_admin_disable_page($set_name) {
  _votingapi_set_action_status($set_name, false);
  drupal_goto('admin/voting/actions');
}

function _votingapi_create_set_code($set_name) {
  $data = cache_get('votingapi_action_sets');
  $action_sets = unserialize($data->data);
  $set = $action_sets[$set_name];
  if ($set) {
    unset($set['name']);
    $code = "\n\$sets = array(\n";
    $code .= "'$set_name' => ";
    ob_start();
    var_export($set);
    $code .= ob_get_clean();
    ob_end_clean();
    $code .= "\n);\n";
  }
  
  return $code;
}

// This one, this is the biggie. 

function votingapi_actions_admin_edit_page($mode = 'add', $set_name = NULL) {
  $form = array();
  $form[] = array(
    '#type' => 'markup',
    '#value' => 'Work in progress. Come again soon!'
  );
  return $form;
}