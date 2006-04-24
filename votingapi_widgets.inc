<?php
/* $Id$ */

/*
 *  This file implements custom form widgets used by VotingAPI for its
 *  configuration screens, and also provides a convenience widget for
 *  radio-button based rating.
 */

function votingapi_elements() {
  // A simple element that maps n points to a list of radio butons.
  // Useful for 'five star' rating widgets and so on.
  $type['rating'] = array(
    '#input' => TRUE,
    '#first' => 1,
    '#points' => 5,
    '#process' => array('expand_rating' => array())
  );
  $type['vote_properties'] = array(
    '#input' => TRUE,
    '#process' => array('expand_vote_properties' => array()),
    '#validate' => array('vote_properties_validate' => array()),
  );
  $type['vote_cache_properties'] = array(
    '#input' => TRUE,
    '#process' => array('expand_vote_cache_properties' => array()),
    '#validate' => array('vote_cache_properties_validate' => array()),
  );
  return $type;
}

/**
 * Roll out a single radios element to a list of radios,
 * using the options array as index.
 */
function expand_rating($element) {
  if ($element['#points'] > 1) {
    for ($i = $element['#first']; $i <= $element['#points']; $i++) {
      $element[$i] = array(
        '#type' => 'radio',
        '#return_value' => $i,
        '#default_value' => $element['#default_value'],
        '#parents' => $element['#parents'],
        '#spawned' => TRUE,
      );
    }
  }
  return $element;
}

function theme_rating($element) {
  if ($element['#title'] || $element['#description']) {
    return theme('form_element', $element['#title'], '<div class="container-inline">' . $element['#children'] . '</div>', $element['#description'], $element['#id'], $element['#required'], form_get_error($element));
  }
  else {
    return '<div class="container-inline">' . $element['#children'] . '</div>';
  }
}


function vote_properties_validate($form) {
  if (isset($form['value_type']['#value'])) {
    $val['value_type'] = trim($form['value_type']['#value']);
  }
  if (isset($form['tag']['#value'])) {
    $val['tag'] = trim($form['tag']['#value']);
  }

  form_set_value($form['tag'], NULL);
  form_set_value($form['value_type'], NULL);
  form_set_value($form, serialize($val));

  return $form;
}


function expand_vote_properties($element) { 
  $element['#tree'] = TRUE;
  $default_value = $element['#default_value'];
  if (isset($default_value) && !is_array($default_value)) {
    // we're in a funky situation where it's probably serialized.
    $default_value = unserialize($default_value);
  }

  $element['value_type'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_value_types(),
    '#default_value' => $default_value['value_type'],
    '#spawned' => TRUE,
  );

  $element['tag'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_tags(),
    '#default_value' => $default_value['tag'],
    '#spawned' => TRUE,
  );

  return $element;
}

function theme_vote_properties($element) {
  if ($element['#title'] || $element['#description']) {
    return theme('form_element', $element['#title'], '<div class="container-inline">' . $element['#children'] . '</div>', $element['#description'], $element['#id'], $element['#required'], form_get_error($element));
  }
  else {
    return '<div class="container-inline">' . $element['#children'] . '</div>';
  }
}

function vote_cache_properties_validate($form) {
  if (isset($form['value_type']['#value'])) {
    $val['value_type'] = trim($form['value_type']['#value']);
  }
  if (isset($form['tag']['#value'])) {
    $val['tag'] = trim($form['tag']['#value']);
  }
  if (isset($form['function']['#value'])) {
    $val['function'] = trim($form['function']['#value']);
  }

  form_set_value($form['tag'], NULL);
  form_set_value($form['value_type'], NULL);
  form_set_value($form['function'], NULL);
  form_set_value($form, serialize($val));

  return $form;
}


function expand_vote_cache_properties($element) { 
  $element['#tree'] = TRUE;
  $default_value = $element['#default_value'];
  if (isset($default_value) && !is_array($default_value)) {
    // we're in a funky situation where it's probably serialized.
    $default_value = unserialize($default_value);
  }

  $element['value_type'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_value_types(),
    '#default_value' => $default_value['value_type'],
    '#spawned' => TRUE,
  );

  $element['tag'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_tags(),
    '#default_value' => $default_value['tag'],
    '#spawned' => TRUE,
  );

  $element['function'] = array(
    '#type' => 'select',
    '#options' => votingapi_cache_functions(),
    '#default_value' => $default_value['function'],
    '#spawned' => TRUE,
  );
  return $element;
}

function theme_vote_cache_properties($element) {
  if ($element['#title'] || $element['#description']) {
    return theme('form_element', $element['#title'], '<div class="container-inline">' . $element['#children'] . '</div>', $element['#description'], $element['#id'], $element['#required'], form_get_error($element));
  }
  else {
    return '<div class="container-inline">' . $element['#children'] . '</div>';
  }
}