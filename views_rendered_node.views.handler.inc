<?php

class views_rendered_node_field extends views_handler_field {
	public function init(&$view, &$options) {
		parent::init($view, $options);

		$this->additional_fields['rendered_node'] = array('table' => 'node', 'field' => 'nid');
	}

	public function option_definition() {
		$options = parent::option_definition();
		$options['view_mode'] = array('default' => 'full');
		$options['bypass_access'] = array('default' => FALSE);

		return $options;
	}

	public function query() {
		$this->add_additional_fields();
	}

	protected function _view_mode_options() {
		$view_mode_options = array();

		$entity_info = entity_get_info('node');
		$view_modes = $entity_info['view modes'];

		if (is_array($view_modes)) {
			backdrop_sort($view_modes, array('label' => SORT_STRING));
		}

		foreach ($view_modes as $key => $info) {
			$view_mode_options[$key] = $info['label'];
		}

		if ($key == 'full') {
			$view_mode_options[$key] = t('Default');
		}

		return $view_mode_options;
	}

	public function options_form(&$form, &$form_state) {
		$form['view_mode'] = array(
			'#type' => 'select',
			'#title' => t('Display mode'),
			'#options' => $this->_view_mode_options(),
			'#default_value' => $this->options['view_mode']
		);

		$form['bypass_access'] = array(
			'#type' => 'checkbox',
			'#title' => t('Bypass access checks'),
			'#description' => t('If enabled, access permissionss for rendering the entity are not checked.'),
			'#default_value' => $this->options['bypass_access']
		);

		parent::options_form($form, $form_state);
	}

	public function render($values) {
		$node = node_load($values->nid);
		return node_view($node, $this->options['view_mode']);
	}
}
