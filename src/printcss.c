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
			      
#include "hypermail.h"
#include "setup.h"

#include "proto.h"

void print_default_css_file(char *filename)
{
    FILE *fp;
  
    if ((fp = fopen(filename, "w")) == NULL) {
        snprintf(errmsg, sizeof(errmsg), "%s \"%s\".", lang[MSG_COULD_NOT_WRITE], filename);
        progerr(errmsg);
    }

    fprintf(fp, "/* default CSS for hypermail mailing list archives */\n");

    fprintf(fp, "\n");
    fprintf(fp, "html {\n");
    fprintf(fp, "    color: black;\n");
    fprintf(fp, "    background-color:  #FFF6E0; /* was: #FFEEC2 */\n");
    fprintf(fp, "    /\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "body {\n");
    fprintf(fp, "    margin: 20px;   /* 40px from base is too high */\n");
    fprintf(fp, "    line-height: 1.4;\n");
    fprintf(fp, "    background: transparent;\n");
    fprintf(fp, "    font-family: sans-serif;\n");
    fprintf(fp, "    color: black;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "#body {background-color:#fff;}\n");
    fprintf(fp, "\n");
    fprintf(fp, "pre {\n");
    fprintf(fp, "    color: black;\n");
    fprintf(fp, "    background-color: white; \n");
    fprintf(fp, "    padding: 0.5em;\n");
    fprintf(fp, "    white-space: pre-wrap;\n");
    fprintf(fp, "    overflow-wrap: break-word;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* Not too big on the heading - they can be long */\n");
    fprintf(fp, "h1 { font-size: 150%% }\n");
    fprintf(fp, "\n");
    fprintf(fp, ".mail, .head { border-bottom:1px solid black;}\n");
    fprintf(fp, "\n");
    fprintf(fp, "map ul {list-style:none;}\n");
    fprintf(fp, "#message-id { font-size: small;}\n");
    fprintf(fp, "address { font-style:inherit ;}\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* don't print the navbars pointing to the next message and indexes */\n");
    fprintf(fp, "@media print {\n");
    fprintf(fp, " #upper, #navbarfoot, #navbar {\n");
    fprintf(fp, "    display: none;\n");
    fprintf(fp, "  }\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, ".heading {font-weight: bold; font-style: italic;}\n");
    fprintf(fp, ".head { border-bottom:1px solid black;}\n");
    fprintf(fp, ".foot { border-top:1px solid black;}\n");
    fprintf(fp, "th {font-style:italic;}\n");
    fprintf(fp, "table { margin-left:2em;}nav ul {list-style:none;}\n");
    fprintf(fp, "#mid { font-size:0.9em;}\n");
    fprintf(fp, "/* #received { float:right;} */\n");
    fprintf(fp, "address { font-style:inherit;}\n");
    fprintf(fp, "\n");
    fprintf(fp, ".quotelev1 {color : #990099;}\n");
    fprintf(fp, ".quotelev2 {color : #ff7700;}\n");
    fprintf(fp, ".quotelev3 {color : #007799;}\n");
    fprintf(fp, ".quotelev4 {color : #95c500;}\n");
    fprintf(fp, ".period {font-weight: bold;}\n");
    fprintf(fp, "\n");
    fprintf(fp, "header nav#upper {overflow: hidden; margin: 1em; }\n");
    fprintf(fp, "header nav#upper ul { list-style: none; padding: 0; margin-left: -4px; }\n");
    fprintf(fp, "header nav#upper ul li { display: inline; white-space: nowrap; }\n");
    fprintf(fp, "header nav#upper ul li:before { content: \" > \"; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "a:link {\n");
    fprintf(fp, "text-underline-position: underline;\n");
    fprintf(fp, "text-decoration-thickness: 1px;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "a:visited {\n");
    fprintf(fp, "color:#754675;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "a:focus {\n");
    fprintf(fp, "outline: 2px solid;\n");
    fprintf(fp, "outline-offset: 1px;\n");
    fprintf(fp, "text-decoration: underline;\n");
    fprintf(fp, "text-decoration-thickness: 1px;\n");
    fprintf(fp, "outline-color:#005A9C;\n");
    fprintf(fp, "cursor :pointer;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "a:hover {\n");
    fprintf(fp, "color:#930;\n");
    fprintf(fp, "text-decoration: underline;\n");
    fprintf(fp, "text-decoration-thickness: 2px;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "table { margin: 1em; }\n");
    fprintf(fp, "thead tr  { color: #FFFFFF; background-color: rgb(0,90,156); }\n");
    fprintf(fp, "tbody tr  { font-family: sans-serif; }\n");
    fprintf(fp, ".head img {float:left; }\n");
    fprintf(fp, "th {font-style:italic;} \n");
    fprintf(fp, "table { border: 0px; border-spacing: 3px; }\n");
    fprintf(fp, "td { padding: 2px; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "/* accessible horizontal menus */\n");
    fprintf(fp, ".cell_message { text-align: center; }\n");
    fprintf(fp, ".cell_period { text-align: right; }\n");
    fprintf(fp, "ul.hmenu_container > li { list-style-type: none; }\n");
    fprintf(fp, "ul.hmenu { font-style: normal; padding: 0; display: inline; }\n");
    fprintf(fp, "ul.hmenu li { display: inline; }\n");
    fprintf(fp, "\n");
    fprintf(fp, "ul.hmenu li {\n");
    fprintf(fp, "    border-right: 1.2px solid black;\n");
    fprintf(fp, "    padding-right: 0.4em;\n");
    fprintf(fp, "    padding-left: 0.4em;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "ul.hmenu li:last-child {\n");
    fprintf(fp, "    border-right: none;\n");
    fprintf(fp, "    padding-right: 0;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "eul.hmenu li {\n");
    fprintf(fp, "    borderAlpha: 0;\n");
    fprintf(fp, "    borderColor: #318bff;\n");
    fprintf(fp, "    borderThickness: 0;\n");
    fprintf(fp, "    gradEndAlpha: 1;\n");
    fprintf(fp, "    gradEndColor: #318bff;\n");
    fprintf(fp, "    padding: 1.03;\n");
    fprintf(fp, "    thickness: 12;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "eul.hmenu li, .widget-title {\n");
    fprintf(fp, "    background-image: linear-gradient(black, black),\n");
    fprintf(fp, "		      linear-gradient(black, black),\n");
    fprintf(fp, "		      linear-gradient(black, black),\n");
    fprintf(fp, "		      linear-gradient(black, black);\n");
    fprintf(fp, "  \n");
    fprintf(fp, "  background-repeat: no-repeat;\n");
    fprintf(fp, "  background-size: 3px 2px;\n");
    fprintf(fp, "  background-position: top left, top right, bottom left, bottom right;\n");
    fprintf(fp, "\n");
    fprintf(fp, "  border: solid black;\n");
    fprintf(fp, "  text-align: justify;\n");
    fprintf(fp, "  border-width: 0 1.5px;\n");
    fprintf(fp, "  /* display: inline-block;*/\n");
    fprintf(fp, "  vertical-align: top;\n");
    fprintf(fp, "  padding: 1px 3px;\n");
    fprintf(fp, "  margin-right: 5px;\n");
    fprintf(fp, "  borderAlpha: 0;\n");
    fprintf(fp, "  borderColor: #318bff;\n");
    fprintf(fp, "  borderThickness: 0;\n");
    fprintf(fp, "  gradEndAlpha: 1;\n");
    fprintf(fp, "  gradEndColor: #318bff;\n");
    fprintf(fp, "  thickness: 12;\n");
    fprintf(fp, "}\n");
    fprintf(fp, "\n");
    fprintf(fp, "eul.hmenu li, .widget-title {\n");
    fprintf(fp, "    background-image: linear-gradient(black, black),\n");
    fprintf(fp, "		      linear-gradient(black, black),\n");
    fprintf(fp, "		      linear-gradient(black, black),\n");
    fprintf(fp, "		      linear-gradient(black, black);\n");
    fprintf(fp, "  \n");
    fprintf(fp, "  background-repeat: no-repeat;\n");
    fprintf(fp, "  background-size: 3px 2px;\n");
    fprintf(fp, "  background-position: top left, top right, bottom left, bottom right;\n");
    fprintf(fp, "\n");
    fprintf(fp, "  border: solid black;\n");
    fprintf(fp, "  text-align: justify;\n");
    fprintf(fp, "  border-width: 0 2px;\n");
    fprintf(fp, "  /* display: inline-block;*/\n");
    fprintf(fp, "  vertical-align: top;\n");
    fprintf(fp, "  padding: 1px 3px;\n");
    fprintf(fp, "  margin-right: 5px;\n");
    fprintf(fp, "}\n");
     
    fclose(fp);
    
    if (chmod(filename, set_filemode) == -1) {
        snprintf(errmsg, sizeof(errmsg), "%s \"%s\": %o.", lang[MSG_CANNOT_CHMOD], filename, set_filemode);
	progerr(errmsg);
    }
    
} /* end print_default_css_file */

