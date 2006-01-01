<?php

// load Smarty library
require('Smarty/Smarty.class.php');

// The setup.php file is a good place to load
// required application library files, and you
// can do that right here. An example:
// require('guestbook/guestbook.lib.php');

class AdtSmarty extends Smarty {

  function AdtSmarty()
  {
    // Class Constructor.
    // These automatically get set with each new instance.

    $this->assign('app_name', 'ADT');

    $this->Smarty();
    $this->template_dir = APP_SMARTY_DATA . 'templates/';
    $this->config_dir = APP_SMARTY_DATA . 'configs/';
    $this->compile_dir = APP_SMARTY_DATA . 'templates_c/';
    $this->cache_dir = APP_SMARTY_DATA . 'cache/';
    $plugins_dir = APP_SMARTY_DATA . 'plugins/';
    array_unshift($this->plugins_dir, $plugins_dir);

    $this->caching = true;
  }
   
  function layout($template)
  {
    if (preg_match("/\.markdown$/", $template))
    {
      $this->load_filter('output', 'markdown');
    }
    $this->assign('tpl_name', $template);
    // Use $template as cache_id
    $this->display('layout.tpl', $template);
  }
}

function createSmarty()
{
   return new AdtSmarty;
}

?>
