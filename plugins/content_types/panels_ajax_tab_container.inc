<?php

$plugin = array(
  'title'            => t('AJAX Tab - Container'),
  'description'      => t('The AJAX content will be loaded within this pane'),
  'single'           => TRUE,
  'category'         => t('AJAX Tabs'),
  'required context' => array(
    new ctools_context_optional(t('Entity Context'), 'entity'),
  ),
  'render callback'  => 'panels_ajax_tab_container_render',
  'edit form'        => 'panels_ajax_tab_container_edit_form'
);

function panels_ajax_tab_container_render($subtype, $conf, $panel_args, &$contexts){
  static $default_tab_container_id = 0;
  ctools_include('context');
  ctools_include('plugins', 'panels');
  $block = new stdClass();
  $block->title ='';
  $block->content = '';

  // Add required JavaScript and CSS
  drupal_add_js(drupal_get_path('module','panels_ajax_tab') . '/panels_ajax_tab.js');

  // Generate the context string
  if (!$contexts[0]->data) {
    $context_string = 'none';
  }
  else {
    $entity_type = $contexts[0]->type[2];
    $entity_id = array_shift(entity_extract_ids($entity_type, $contexts[0]->data));
    $context_string = $entity_type . ':' . $entity_id;
  }

  // Load the mini-panels
  $minipanels = array();
  foreach ($conf['mini_panels'] as $panel_name) {
    $minipanels[] = panels_mini_load($panel_name);
  }

  // Get the tab-container-id
  if ($conf['container_id'] && !$conf['diplay_tabs']) {
    $tab_container_id = $conf['container_id'];
  }
  else {
    $tab_container_id = $default_tab_container_id;
  }

  // Render the tabs
  if ($conf['display_tabs'] || !isset($conf['display_tabs'])) {
    $block->content .= theme('panels_ajax_tab_tabs', array('minipanels' => $minipanels, 'tab_container_id' => $tab_container_id, 'context_string' => $context_string));
  }

  // Render the container
  $block->content .= theme('panels_ajax_tab_container', array('minipanels' => $minipanels, 'tab_container_id' => $tab_container_id, 'context_string' => $context_string));

  $default_tab_container_id++;

  return $block;
}

function panels_ajax_tab_container_edit_form($form, &$form_state){
  $conf = $form_state['conf'];

  $panels = panels_mini_load_all();

  $options = array();
  foreach ($panels as $panel) {
    $options[$panel->name] = $panel->admin_title;
  }


  $form['display_tabs'] = array(
    '#title' => t('Display Tabs'),
    '#type' => 'checkbox',
    '#default_value' => isset($conf['display_tabs']) ? $conf['display_tabs'] : 1,
  );
  $form['mini_panels'] = array(
    '#title' => t('Mini Panels'),
    '#type' => 'checkboxes',
    '#options' => $options,
    '#description' => 'Select the Mini Panels to be used as AJAX tabs',
    '#default_value' => $conf['mini_panels'],
    '#states' => array(
      'invisible' => array(
        ':input[name="display_tabs"]' => array('checked' => FALSE),
      ),
    )
  );
  $form['container_id'] = array(
    '#title' => t('Container ID'),
    '#type' => 'textfield',
    '#description' => 'If you are using tabs in seperate pane, you must supply a unique container id string.',
    '#default_value' => $conf['container_id'],
    '#states' => array(
      'invisible' => array(
        ':input[name="display_tabs"]' => array('checked' => TRUE),
      ),
    )
  );

  return $form;
}

function panels_ajax_tab_container_edit_form_submit(&$form, &$form_state){
  $form_state['conf']['mini_panels'] = array_filter($form_state['values']['mini_panels']);
  $form_state['conf']['display_tabs'] = $form_state['values']['display_tabs'];
  $form_state['conf']['container_id'] = $form_state['values']['container_id'];
}
