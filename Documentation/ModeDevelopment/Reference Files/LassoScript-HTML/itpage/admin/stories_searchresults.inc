<?Lassoscript 
// Last modified 1/15/09 by ECL, Landmann InterActive

// FUNCTIONALITY
// This establishes the queries and results pages for the admin Stories area
// The controlling record is ID from the stories table, which is passed as the action_param "ID"

// CHANGE NOTES
// 1/15/09
// First implementation

// Start the session
Session_Start: -Name=$svSessionAdminName, -Expires=$svSessionTimeout;

If: !(Var_Defined:'SQLQueryStories'); Session_AddVariable: -Name=$svSessionAdminName, 'SQLQueryStories'; /If;

// Debugging
// Var:'svDebug' = 'Y';

// Setting Variables for Nav Control
Var:'vNewQuery' = (Action_Param:'NewQuery');
Var:'Records_Skipped' = (Action_Param:'skip');
Var:'This_Group' = (Action_Param:'group');
Var:'Records_Per_Page' = '10';
Var:'SkipCount' = integer;
Var:'Records_Found' = integer;
Var:'Group_Size' = (Math_Mult:(Var:'Records_Per_Page'),5);

If: $svDebug == 'Y';
	'<p class="debug"><strong>/admin/stories_searchresults</strong></p>\n';
/If;

// Create the query if NewQuery = Y
If: Var:'vNewQuery' == 'Y';

	Var:'vSearchFor' = (Action_Param:'SearchFor');
	Var:'vActive' = (Action_Param:'Active');

	Var:'SQLSearchFor' = string;
	Var:'SQLActive' = string;

	Var:'SQLQueryStories' = '';
	Var:'SQLQuerySuffix' = '';
	Var:'svSearch_Params' = '';
	Var:'svSearch_ParamsURL' = '';

	// Include the code for building the search params phrase
	Include:($svLibsPath)'searchparamsstories_build.inc';

/If;

// Define the suffix (same for any query)
Var:'SQLQuerySuffix' = ' ORDER BY Story_Head';

// Constructing the query
If: (Var:'SQLQueryStories') == '';

	var:'SQLQueryStories' = (string);
	var:'SQLQueryLimit' = (string);
	Var:'SQLQueryStories' = 'SELECT * FROM ' $svSiteDatabase '.' $svStoriesTable ' WHERE ';

	// SEARCH FOR
	If: Var:'vSearchFor' != '';
		Var:'SQLSearchFor' = 'Story_Head LIKE "%' $vSearchFor '%" OR
		Story_Name LIKE "%' $vSearchFor '%" OR 
		Story_Comment LIKE "%' $vSearchFor '%"';

		$SQLQueryStories += $SQLSearchFor;
		If: Var:'vActive' != '';
			$SQLQueryStories += ' AND ';
		/If;
	/If;

	// ACTIVE
	If: Var:'vActive' != '';
		Var:'SQLActive' = 'Active = "' $vActive '"';
		$SQLQueryStories += $SQLActive;
	/If;


	// Add suffix
	$SQLQueryStories += $SQLQuerySuffix;

	// SEARCHES EVERYTHING
	// If no search params passed, redefine the query to get everything
	If: ($vSearchFor == '') && ($vActive == '');
		Var:'SQLQueryStories' = 'SELECT * FROM ' $svSiteDatabase '.' $svStoriesTable ' ORDER BY Story_Head ASC';
	/If;

	If: $svDebug == 'Y';
		'116: vSearchFor = ' (Var:'vSearchFor') '<br>\n';
		'116: vActive = ' (Var:'vActive') '<br>\n';
	/If;

/If;
	
// Performing Search
Inline: $IV_Stories, 
	-skiprecords=(var:'Records_Skipped'),
	-MaxRecords=(Var:'Records_Per_Page'),
	-SQL=$SQLQueryStories;

	If: $svDebug == 'Y';
		'131: Error = ' (Error_CurrentError) '<br>\n';
		'131: Found_Count = ' (Found_Count) '<br>\n';
	/If;
	
	// If no records found, immediate redirect with error
	If: (Found_Count) == 0;
		Var:'vError'='2000';
		Var:'URL' = ('setup_search.lasso?Error='$vError'&'(Client_POSTArgs));
		If: $svDebug == 'Y';
			'140: URL: ' $URL '<br>\n';
			'<a href="'$URL'">' $URL '</a><br>\n';
		Else;
			Redirect_URL: $URL;
		/If;
	/If;

	// If only one record, redirect immediately to editrecord
	// This shortcut allows users to skip past the search results page if there is only one record found
	If: (Found_Count) == 1;
		Var:'URL' = ('setup_editrecord.lasso?ID='(Field:'ID')'&DataType='(Var:'vDatatype')'&New=Y');
		If: $svDebug == 'Y';
			'155: URL: ' $URL '<br>\n';
			'<a href="'$URL'">' $URL '</a><br>\n';
		Else;
			Redirect_URL: $URL;
		/If;
	/If;
?>
<!-- Search Results-->
<table width="525">
	<tr>
		<td colspan="5" valign="top">
			<div align="left">
				<h2>[LI_ShowIconByDataType]&nbsp;&nbsp;Stories Search Results</h2>
				<p><font color="red">[Var:'svSearch_Params', -EncodeNone]</font>&nbsp;&nbsp;
				<a href="http://[$svDomain]/admin/setup_search.lasso[$svSearch_ParamsURL]">Search Again</a></p>
			</div>
		</td>
	</tr>
	<tr bgcolor="#000000">
		<td width="20" valign="middle" class="tablehead_10_white"><strong>ID</strong></td>
		<td width="150" valign="middle" class="tablehead_10_white"><strong>Headline</strong></td>
		<td width="370" valign="middle" class="tablehead_10_white"><strong>Comment</strong></td>
		<td width="20" valign="middle" class="tablehead_10_white"><strong>Active</strong></td>
	</tr>
<?Lassoscript
Records;
	Var:'vID' = (Field:'ID');
	Var:'vStory_Head' = (Field:'Story_Head');
	Var:'vStory_Comment' = (Field:'Story_Comment');
	Var:'vActive' = (Field:'Active');
?>	<tr <?Lassoscript If: (Loop_Count) %2 == 0; 'bgcolor="#F5F5F5"'; Else; 'bgcolor="#FFFFFF"'; /If; ?>>
		<td valign="top" width="20" class="tabletext_10_black">
			<strong><a href="/admin/setup_editrecord.lasso?ID=[Var:'vID']&DataType=[Var:'vDatatype']&New=Y">[Var:'vID']</a></strong></td>
		<td valign="top" width="150" class="tabletext_10_black">
			<strong>[$vStory_Head]</strong>
		</td>
		<td valign="top" width="270" class="tabletext_10_black">
			[$vStory_Comment]
		</td>
		<td valign="top" width="20" class="tabletext_10_black">
			[$vActive]
		</td>
	</tr>
[/Records]
	<tr>
		<td colspan="5" valign="top" bgcolor="#cccccc">&nbsp;</td>
	</tr>
	<tr>
		<td colspan="5" valign="middle" align="left" bgcolor="#f5f5f5">
			[Include:($svLibsPath)'nav_control.inc']
		</td>
	</tr>
</table>
[/Inline]
<br>
