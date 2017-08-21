<?php

if (!empty($_GET['pd'])) {
    $pd = strtoupper($_GET['pd']);
} else
    $pd = "AP89";

//$Comm = "sudo /usr/bin/svn list https://svn.maxim-ic.com/svn/ets/test_programs/" . $pd . "/" . $pd . "_FT/Trunk > /var/www/html/test3/list.txt";
$Comm = "sudo /usr/bin/svn list https://svn.maxim-ic.com/svn/ets/test_programs > /var/www/html/test3/list.txt";

exec($Comm);

$Comm = "sudo /usr/bin/svn export https://svn.maxim-ic.com/svn/ets/test_programs/" . $pd . "/" . $pd . "_FT/Trunk/" . $pd . "_FT.cpp /var/www/html/test3/";

exec($Comm);

$file     = "list.txt";
$handlr   = fopen($file, "r");
$contents = fread($handlr, filesize($file));
fclose($handlr);
echo "<pre>";
echo $contents;
echo "</pre>";

?>
