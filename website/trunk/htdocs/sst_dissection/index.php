<?php

require('setup.php');

$smarty = createSmarty();
$smarty->assign('title', "SST Dissection");
$smarty->layout('sst_dissection.markdown');

?>