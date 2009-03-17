<?
include('snekgraph.inc');

MyConnect();
$query="select * from configvars";
mySQLQueryTable($query,5);

?>
