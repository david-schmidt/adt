<?php

require("markdown.lib.php");

function smarty_outputfilter_markdown($source, &$smarty)
{
  return Markdown($source);
}

?>