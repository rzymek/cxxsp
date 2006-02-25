<%@ page import="store.Category,store.Article,
                 java.util.ArrayList,
                 store.Product" %>
<%
    ArrayList categories = (ArrayList) request.getAttribute("categories");
    ArrayList weekly = (ArrayList) request.getAttribute("weekly");
    ArrayList featured = (ArrayList) request.getAttribute("featured");
    ArrayList wereReading = (ArrayList) request.getAttribute("wereReading");
    ArrayList notable = (ArrayList) request.getAttribute("notable");
    ArrayList recomended = (ArrayList) request.getAttribute("recomended");
%>
<html>
<head>
<title>Default</title>
<link rel="stylesheet" type="text/css" href="style/Style.css">
</head>
<body bgcolor="#FFFFFF" link="#000000" alink="#FF0000" vlink="#000000" text="#000000" class="NoisePageBODY">
<%@ include file="Header.jsp" %>
<table border="0" cellspacing="5" width="100%">
  <tr valign="top">
    <td width="20%">
      <form action="Products.sxx" method="get">
        <font class="NoiseSectionHeaderFont">Search Products </font>
        <table border="0" cellpadding="2" cellspacing="1" width=100%>
          <tr>
            <td>
              <select class="NoiseSelect" name="category_id">
                <option selected value="">All</option>
                  <% for(int i=0;i<categories.size();++i) { %>
                    <option value="<%=((Category)categories.get(i)).id%>"><%=((Category)categories.get(i)).name%></option>
                  <% } %>
              </select>
            </td>
          </tr>
          <tr>
            <td><input class="NoiseInput" maxlength="250" name="s_keyword">&nbsp;</td>
          </tr>
          <tr>
            <td nowrap>
              <input class="NoiseButton" type="submit" value="Search">&nbsp; </td>
          </tr></form>
        </table>
      <br><a class="NoiseDataLink" href="Search.sxx">Advanced search</a>
      <br><br>
      <table border="0" cellpadding="5" cellspacing="0" width="100%">
        <tr>
            <td bgcolor="#395A9C"><font color="#FFFFFF"><B>Categories</B></font></td>
        </tr>
      </table>
      <table border="0" cellpadding="1" cellspacing="0" width=100%>
      <tr>
        <td bgcolor="#395A9C">
            <table border="0" cellpadding="3" cellspacing="0" width=100%>
              <% for(int i=0;i<categories.size();++i) { %>
                <tr>
                    <td class="NoiseDataTD"><a class="NoiseDataLink" href="Products.sxx?category_id=<%=((Category)categories.get(i)).id%>"><%=((Category)categories.get(i)).name%></a>&nbsp;</td>
                </tr>
              <% } %>
              <% if(categories.isEmpty()) { %>
            <tr>
              <td class="NoiseDataTD">No records&nbsp;</td>
            </tr>
               <% } %>
            </table>

        </td>
      </tr>
      </table>
      <br>
      <table border="0" cellpadding="5" cellspacing="0" width=100%>
      <tr>
        <td bgcolor="#395A9C"><font color="#FFFFFF"><B>Weekly Special</B></font></td>
      </tr>
      </table>
      <table border="0" cellpadding="1" cellspacing="0" width=100%>
        <tr>
            <td bgcolor="#395A9C">
              <table border="0" cellpadding="3" cellspacing="0" width=100%>
               <% for(int i=0;i<weekly.size();++i) { %>
                    <tr>
                      <td class="NoiseDataTD"><%= ((Article)weekly.get(i)).title %>&nbsp;</td>
                    </tr>
                    <tr>
                      <td class="NoiseDataTD"><%= ((Article)weekly.get(i)).desc %>&nbsp;</td>
                    </tr>
               <% } if(weekly.isEmpty()) { %>
                    <tr>
                      <td class="NoiseDataTD">No records&nbsp;</td>
                    </tr>
               <% } %>
              </table>
             </td>
        </tr>
      </table>
      </p>
    </td>
    <td>
        <% if(request.getAttribute("error") != null) { %>
        <table><tr>
          <td colspan="2" class="NoiseErrorDataTD"><%= request.getAttribute("error") %></td>
        </tr></table>
        <% } %>      
      <font class="NoiseFormHeaderFont">What We're Reading</font>
      <table border="0" cellpadding="5" cellspacing="1">
      <% for(int i=0;i<wereReading.size(); ++i) { %>
        <tr>
          <td valign="top" width="125"><a href="<%=((Article)wereReading.get(i)).link%>"><img class="NoiseInput" src="<%=((Article)wereReading.get(i)).img%>" border="0"></a></td>
          <td valign="top"><p><a class="NoiseCaptionLink" href="<%=((Article)wereReading.get(i)).link%>"><%=((Article)wereReading.get(i)).title%></a>
              <p><%=((Article)wereReading.get(i)).desc%></td>
       </tr>
      <% } if(wereReading.isEmpty()) { %>
        <tr>
          <td>No records</td>
        </tr>
      <% } %>
      </table>
      <br><br>
      <font class="NoiseFormHeaderFont">This Week's Featured Books</font>
      <table border="0" cellpadding="5" cellspacing="1" class="NoiseFormTABLE">
      <% for(int i=0;i<featured.size(); ++i) { %>
        <tr>
          <td valign="top" width="125"><a href="<%=((Article)featured.get(i)).link%>"><img class="NoiseInput" src="<%=((Article)featured.get(i)).img%>" border="0"></a></td>
          <td valign="top"><p><a class="NoiseCaptionLink" href="<%=((Article)featured.get(i)).link%>"><%=((Article)featured.get(i)).title%></a>
              <p><%=((Article)featured.get(i)).desc%></td>
        </tr>
      <% } if(featured.isEmpty()) { %>
        <tr>
          <td>No records</td>
        </tr>
      <% } %>
      </table>
      <br><br>
      <font class="NoiseFormHeaderFont">New &amp; Notable</font>
      <table border="0" cellpadding="5" cellspacing="1">
      <% for(int i=0;i<notable.size(); ++i) { %>
        <tr>
          <td valign="top" width="125"><a href="<%=((Article)notable.get(i)).link%>"><img class="NoiseInput" src="<%=((Article)notable.get(i)).img%>" border="0"></a></td>
          <td valign="top"><p><a class="NoiseCaptionLink" href="<%=((Article)notable.get(i)).link%>"><%=((Article)notable.get(i)).title%></a>
              <p><%=((Article)notable.get(i)).desc%></td>
        </tr>
      <% } if(notable.isEmpty()) { %>
        <tr>
          <td>No records</td>
        </tr>
      <% } %>
      </table>
      <br><br>
      <!-- BEGIN Grid new --><!-- END Grid new --></p>
    </td>
        <td>
            <p><font class="NoiseSectionHeaderFont">Recomended Titles</font>
            <p>
               <%if(!recomended.isEmpty()) { %>
                <table border="0" cellpadding="1" cellspacing="0" width=100%>
                <tr>
                <td bgcolor="#395A9C">
                  <% for(int i=0;i<recomended.size();++i) { String link = "ProductDetails.sxx?product_id="; %>
                    <table border="0" cellpadding="5" cellspacing="0">
                    <tr>
                    <td class="NoiseAltDataTD" colspan="2">
                        <a class="NoiseDataLink" href="<%=link%><%=((Product)recomended.get(i)).product_id%>"><%=((Product)recomended.get(i)).name%></a>
                    </td>
                    </tr>
                    <tr>
                    <td class="NoiseAltDataTD">
                        <a href="<%=link%><%=((Product)recomended.get(i)).product_id%>"><img class="NoiseInput" src="<%=((Product)recomended.get(i)).img%>" border="0"></a>
                    </td>
                    <td class="NoiseAltDataTD" valign="top"><font class="NoiseHighlightFont">Price: <%=((Product)recomended.get(i)).price%></font></td>
                    </tr>
                    </table>
                  <% } %>
                </td>
                </tr>
                </table>
              <% }else{ %>
                <table border="0" cellpadding="1" cellspacing="0" width=100%>
                <tr>
                <td bgcolor="#395A9C">

                    <table border="0" cellpadding="5" cellspacing="0">
                    <tr>
                        <td class="NoiseAltDataTD">No records</td>
                        </tr>
                    </table>

                </td>
                </tr>
                </table>
              <% } %>
        </td>
    </tr>
</table>
<%@ include file="Footer.jsp" %>
</body>
</html>
