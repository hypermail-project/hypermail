/* 
** This program and library is free software; you can redistribute it and/or 
** modify it under the terms of the GNU (Library) General Public License 
** as published by the Free Software Foundation; either version 2 
** of the License, or any later version. 
** 
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of 
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
** GNU (Library) General Public License for more details. 
** 
** You should have received a copy of the GNU (Library) General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA 
*/

/* THIS FILE IS GENERATED BY RUNNING contrib/css_to_c.pl ON A CSS FILE
** DO NOT EDIT THIS FILE BY HAND; RATHER EDIT docs/hypermail.css TO
** REGENERATE IT. IF YO WANT TO CHANGE THE C CODE ITSELF, PLEASE EDIT
** THE css_to_c.pl SCRIPT
*/
			      
#include "hypermail.h"
#include "setup.h"

#include "proto.h"

void print_default_css_file(char *filename)
{
    FILE *fp;
  
    if ((fp = fopen(filename, "w")) == NULL) {
        trio_snprintf(errmsg, sizeof(errmsg), "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
        progerr(errmsg);
    }

    fprintf(fp, "/*\n");
    fprintf(fp, "** This program and library is free software; you can redistribute it and/or \n");
    fprintf(fp, "** modify it under the terms of the GNU (Library) General Public License \n");
    fprintf(fp, "** as published by the Free Software Foundation; either version 2 \n");
    fprintf(fp, "** of the License, or any later version. \n");
    fprintf(fp, "** \n");
    fprintf(fp, "** This program is distributed in the hope that it will be useful, \n");
    fprintf(fp, "** but WITHOUT ANY WARRANTY; without even the implied warranty of \n");
    fprintf(fp, "** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the \n");
    fprintf(fp, "** GNU (Library) General Public License for more details. \n");
    fprintf(fp, "** \n");
    fprintf(fp, "** You should have received a copy of the GNU (Library) General Public License\n");
    fprintf(fp, "** along with this program; if not, write to the Free Software \n");
    fprintf(fp, "** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA \n");
    fprintf(fp, "**/\n");
    fprintf(fp, "\n");
    fprintf(fp, "/*\n");
    fprintf(fp, "** Default stylesheet for hypermail archives\n");
    fprintf(fp, "** \n");
    fprintf(fp, "** Last revised: 02/May/2023\n");
    fprintf(fp, "**/\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* Use this CSS rule to choose the background color\n");
    fprintf(fp, "   for your archive */\n");
    fprintf(fp, "html {\n");
    fprintf(fp, "  color: black;\n");
    fprintf(fp, "  background: #e2edfe;\n");
    fprintf(fp, "  /** you may also try #FFF6E0, #FFFFF6 or #FFEEC2.\n");
    fprintf(fp, "   ** For other color combinations, make sure you choices pass\n");
    fprintf(fp, "   ** the WAI contrast criteria\n");
    fprintf(fp, "   ** by using https://webaim.org/resources/contrastchecker/ */\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* wai-menu.css */\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* accessible breadcrumbs */\n");
    fprintf(fp, "nav.breadcrumb ul { padding: 0; }\n");
    fprintf(fp, "nav.breadcrumb ul li { display: inline;}\n");
    fprintf(fp, "nav.breadcrumb ul li+li::before { content: \" > \"; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* accessible horizontal menus */\n");
    fprintf(fp, "ul.hmenu_container { margin-left: 1em; padding-left: 0; }\n");
    fprintf(fp, "ul.hmenu_container > li { list-style-type: none; }\n");
    fprintf(fp, "ul.hmenu { font-style: normal; padding: 0; display: inline; }\n");
    fprintf(fp, "ul.hmenu li { display: inline; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "ul.hmenu li {\n");
    fprintf(fp, "    border-right: 1.2px solid black;\n");
    fprintf(fp, "    padding-right: 0.4em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "ul.hmenu li:last-child {\n");
    fprintf(fp, "    border-right: none;\n");
    fprintf(fp, "    padding-right: 0;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* replacement for dfn */\n");
    fprintf(fp, ".heading { font-weight: bold; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "h2.heading, h2.theading {\n");
    fprintf(fp, "    font-size: inherit;\n");
    fprintf(fp, "    font-weight: normal;\n");
    fprintf(fp, "    margin-top: 0.2em;\n");
    fprintf(fp, "    margin-bottom: 0;\n");
    fprintf(fp, "    display: list-item;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "h2.theading:before  {\n");
    fprintf(fp, "    width:10px;\n");
    fprintf(fp, "    height:10px;\n");
    fprintf(fp, "    border-radius:50%%;\n");
    fprintf(fp, "    background: #b83b3b;\n");
    fprintf(fp, "    display:inline-block;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "h2.theading + ul {\n");
    fprintf(fp, "    list-style-type: disc;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* default settings for narrow screens (smaller headings and padding) */\n");
    fprintf(fp, "body {\n");
    fprintf(fp, "    font-family: sans-serif;\n");
    fprintf(fp, "    margin: 1em;\n");
    fprintf(fp, "    line-height: 1.4;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "h1 {\n");
    fprintf(fp, "    font-size: 1.25em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "h2 {\n");
    fprintf(fp, "    font-size: 1.125em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "h3 {\n");
    fprintf(fp, "    font-size: 1em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".messages-list {\n");
    fprintf(fp, "    padding-top: 1em;\n");
    fprintf(fp, "    margin-left: 1em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, ".messages-list ul {\n");
    fprintf(fp, "    margin: 0;\n");
    fprintf(fp, "    padding-left: 1em;\n");
    fprintf(fp, "    list-style-type: disc;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* bigger headings and padding on wider screens (desktops, tablets) */\n");
    fprintf(fp, "@media screen and (min-width: 48em) {\n");
    fprintf(fp, "    body {\n");
    fprintf(fp, "	margin: 1em 2.5em;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    h1 {\n");
    fprintf(fp, "	font-size: 1.5em;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    h2 {\n");
    fprintf(fp, "	font-size: 1.25em;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    h3 {\n");
    fprintf(fp, "	font-size: 1.125em;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    ul.hmenu_container {\n");
    fprintf(fp, "	padding-left: 1.5em;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    .messages-list {\n");
    fprintf(fp, "	padding-top: 1.5em;\n");
    fprintf(fp, "	margin-left: 2.5em;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "    .messages-list ul {\n");
    fprintf(fp, "	padding-left: 2em;\n");
    fprintf(fp, "    }\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "ul.footer_hmenu { padding: 0; display: inline; }\n");
    fprintf(fp, "ul.footer_hmenu::after { content: \"\"; clear: both; display: block; }\n");
    fprintf(fp, "ul.footer_hmenu > li { list-style-type: none; }\n");
    fprintf(fp, "ul.footer_hmenu li { display: inline; }\n");
    fprintf(fp, "ul.footer_hmenu li.footer_admin { float: right; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* make links visible thru focus or hover */\n");
    fprintf(fp, "a:link {\n");
    fprintf(fp, "    text-decoration-thickness: 1px;\n");
    fprintf(fp, "    text-underline-offset: 2px;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "a:visited {\n");
    fprintf(fp, "    color:#754675\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "a:focus {\n");
    fprintf(fp, "    outline: 2px solid;\n");
    fprintf(fp, "    outline-offset: 1px;\n");
    fprintf(fp, "    text-decoration: underline;\n");
    fprintf(fp, "    text-decoration-thickness: 1px;\n");
    fprintf(fp, "    outline-color:#005A9C;\n");
    fprintf(fp, "    cursor :pointer;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "a:hover {\n");
    fprintf(fp, "    color:#930;\n");
    fprintf(fp, "    text-decoration: underline;\n");
    fprintf(fp, "    text-decoration-thickness: 2px;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "a:active {\n");
    fprintf(fp, "    /* outline: 0; */\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* mainindex-wai.css */\n");
    fprintf(fp, "\n");
    fprintf(fp, "table { margin: 1em; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* Table head -- should be w3c blue */\n");
    fprintf(fp, "thead tr  { color: #FFFFFF; background-color: rgb(0,90,156); }\n");
    fprintf(fp, "\n");
    fprintf(fp, "tbody tr  { font-family: sans-serif; }\n");
    fprintf(fp, "\n");
    fprintf(fp, ".head img {float:left;}\n");
    fprintf(fp, ".head p {clear: left;}\n");
    fprintf(fp, "/* th {font-style:italic;} */\n");
    fprintf(fp, "nav ul { list-style-type: none; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "table { border: 0px; border-spacing: 3px; }\n");
    fprintf(fp, "td { padding: 2px; }\n");
    fprintf(fp, "/* accessible horizontal menus */\n");
    fprintf(fp, ".cell_message { text-align: center; }\n");
    fprintf(fp, ".cell_period { text-align: right; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "a.sub:link { color: green; }\n");
    fprintf(fp, "a.unsub:link {color: #b22222; }\n");
    fprintf(fp, "\n");
    fprintf(fp, ".header .important { padding: 0.5em 1em; }\n");
    fprintf(fp, "\n");
    fprintf(fp, ".visually_hidden {\n");
    fprintf(fp, "    border: 0;\n");
    fprintf(fp, "    clip: rect(0 0 0 0);\n");
    fprintf(fp, "    height: 1px;\n");
    fprintf(fp, "    margin: -1px;\n");
    fprintf(fp, "    overflow: hidden;\n");
    fprintf(fp, "    padding: 0;\n");
    fprintf(fp, "    position: absolute;\n");
    fprintf(fp, "    width: 1px;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, ".restricted:after {\n");
    fprintf(fp, "    content: \" (restricted)\";\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "body.mainindex .head {\n");
    fprintf(fp, "  border-bottom: none;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* messagelist-wai.css */\n");
    fprintf(fp, "\n");
    fprintf(fp, "dfn {font-weight: bold;}\n");
    fprintf(fp, "nav ul {list-style:none;}\n");
    fprintf(fp, ".messages-list {\n");
    fprintf(fp, "  border-bottom:thin solid black;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* message.css */\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* Leave message itself white but rest of metadata very pale grey.\n");
    fprintf(fp, "** This makes it more legible.\n");
    fprintf(fp, "** it also by analogy points out that the content of the message\n");
    fprintf(fp, "** someone else's, not W3C material necessarily -- we just run the list\n");
    fprintf(fp, "*/\n");
    fprintf(fp, "\n");
    fprintf(fp, "pre {\n");
    fprintf(fp, "    color: black;\n");
    fprintf(fp, "    white-space: pre-wrap;\n");
    fprintf(fp, "    overflow-wrap: break-word;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* want to be able to identify the author and style it\n");
    fprintf(fp, "** but can't\n");
    fprintf(fp, "*/\n");
    fprintf(fp, "\n");
    fprintf(fp, "dfn {font-weight: bold;}\n");
    fprintf(fp, ".mail, .head { border-bottom:1px solid black;}\n");
    fprintf(fp, "map ul {list-style:none;}\n");
    fprintf(fp, "#message-id { font-size: small;}\n");
    fprintf(fp, "address { font-style:inherit ;}\n");
    fprintf(fp, "\n");
    fprintf(fp, "@media print {\n");
    fprintf(fp, " #upper, #navbarfoot, #navbar {\n");
    fprintf(fp, "    display:none;\n");
    fprintf(fp, "  }\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* for edited messages */\n");
    fprintf(fp, "div.edit {\n");
    fprintf(fp, " background: #CCFF99;\n");
    fprintf(fp, " padding: 0.25em 2em 0.25em 1em;\n");
    fprintf(fp, " margin-top: 0.25em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "p.editmes{\n");
    fprintf(fp, " font-style: italic;\n");
    fprintf(fp, " font-size: small;\n");
    fprintf(fp, " padding: 0;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "p.editfooter{\n");
    fprintf(fp, " font-size: small;\n");
    fprintf(fp, " padding: 0;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* for deleted messages */\n");
    fprintf(fp, "div.spam {\n");
    fprintf(fp, " background: #aaffaa;\n");
    fprintf(fp, " padding: 1em 2em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "p.spammes{\n");
    fprintf(fp, " font-size: x-large;\n");
    fprintf(fp, " font-weight: bold;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "p.spamfooter{\n");
    fprintf(fp, " font-size: small;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* updates added by Gerald 27 Apr 2023 */\n");
    fprintf(fp, "\n");
    fprintf(fp, ".message-forwarded {\n");
    fprintf(fp, "    padding: 0.5em 1em;\n");
    fprintf(fp, "    border-left: 4px solid #999;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, ".message-forwarded ul.headers {\n");
    fprintf(fp, "    margin: 0;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, ".message-forwarded pre {\n");
    fprintf(fp, "    padding: 0;\n");
    fprintf(fp, "    margin-bottom: 0.25em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "section section {\n");
    fprintf(fp, "    text-indent: 0;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "ul.headers {\n");
    fprintf(fp, "    list-style: none;\n");
    fprintf(fp, "    padding-left: 0em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, ".attachment-links img {\n");
    fprintf(fp, "    padding: 0.5em 0;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "img {\n");
    fprintf(fp, "    max-width: 100%%;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "section.attachment-links ul {\n");
    fprintf(fp, "    padding-left: 1.5em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* experimental: */\n");
    fprintf(fp, "\n");
    fprintf(fp, ".mail {\n");
    fprintf(fp, "  background: white;\n");
    fprintf(fp, "  padding-left: 0.75em;\n");
    fprintf(fp, "  padding-top: 0.025em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* the following wo rules apply when you're using the \n");
    fprintf(fp, "   yearly_index config option */\n");
    fprintf(fp, ".summary-year {\n");
    fprintf(fp, "    padding-top: 1em;\n");
    fprintf(fp, "    padding-bottom: 1em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "main.summary-year th.cell_period + th,\n");
    fprintf(fp, "main.summary-year td.cell_message + td {\n");
    fprintf(fp, "    padding-left: 0.5em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* the following rules are used to set the style of quotes\n");
    fprintf(fp, "   when you're using the 'showhtml' config option with\n");
    fprintf(fp, "   values 1 or 2 */\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* use this option to specify the style for a quote line */\n");
    fprintf(fp, ".quote {\n");
    fprintf(fp, "/* enable this one to change the style to italic to emulate\n");
    fprintf(fp, "   the deprecated iquote config option */\n");
    fprintf(fp, "    font-style: italic;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* add more entries or separate this rule into\n");
    fprintf(fp, "   multiple ones if you want a specific style for\n");
    fprintf(fp, "   quotes depending on their level */	\n");
    fprintf(fp, ".quotelev1, .quotelev2, .quotelev3, .quotelev5, .quotelev6 {\n");
    fprintf(fp, "}\n");
     
    fclose(fp);
    
    if (chmod(filename, set_filemode) == -1) {
        trio_snprintf(errmsg, sizeof(errmsg), "%s \"%s\": %o.", lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }
    
} /* end print_default_css_file */

