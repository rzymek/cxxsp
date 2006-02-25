<%@ page import="store.Category,
                 java.util.ArrayList" %>
<html>
<head>
<title>AdvSearch</title>
<link rel="stylesheet" type="text/css" href="style/Style.css">
</head>
<body bgcolor="#FFFFFF" link="#000000" alink="#FF0000" vlink="#000000" text="#000000" class="NoisePageBODY">
<%@ include file="Header.jsp" %>
<form action="Products.sxx" method="get">
  <font class="NoiseFormHeaderFont">Search Products </font>
  <table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE">
    <% if(request.getAttribute("error")!=null) { %>
    <tr>
      <td colspan="2" class="NoiseErrorDataTD"><%= request.getAttribute("error") %></td>
    </tr>
    <% } %>
    <tr>
      <td class="NoiseFieldCaptionTD">Category</td>
      <td class="NoiseDataTD">
        <select name="category_id" class="NoiseSelect">
          <option value="" selected>All</option>
          <% 
            ArrayList categories = (ArrayList) request.getAttribute("categories");
            for(int i=0;i<categories.size();++i) {
          %>
          <option value="<%=((Category)categories.get(i)).id%>"><%=((Category)categories.get(i)).name%></option>
          <% } %>                
        </select>
        &nbsp;</td>
    </tr>
    <tr>
      <td class="NoiseFieldCaptionTD">Keyword</td>
      <td class="NoiseDataTD"><input class="NoiseInput" maxlength="250" name="s_keyword">&nbsp;</td>
    </tr>
    <tr>
      <td class="NoiseFieldCaptionTD">Price more then</td>
      <td class="NoiseDataTD"><input class="NoiseInput" maxlength="20" name="price_from"></td>
    </tr>
    <tr>
      <td class="NoiseFieldCaptionTD">Price less then</td>
      <td class="NoiseDataTD"><input class="NoiseInput" maxlength="20" name="price_to"></td>
    </tr>
    <tr>
      <td class="NoiseFieldCaptionTD">Order By</td>
      <td class="NoiseDataTD">
        <select class="NoiseSelect" name="productsOrder">
          <option selected value="">Select Column</option>
          <option value="product">Product</option>
          <option value="price">Price</option>
        </select>
		
        <select name="productsDir" class="NoiseSelect">
          <option value="">Select Direction</option>
          <option value="ASC">Ascending</option>
          <option value="DESC">Descending</option>        
        </select>

      </td>
    </tr>
    <tr>
      <td class="NoiseFieldCaptionTD">Records per page</td>
      <td class="NoiseDataTD">
        <select class="NoiseSelect" name="productsPageSize">
          <option selected value="">Select Value</option>
          <option value="10">10</option>
          <option value="25">25</option>
          <option value="50">50</option>
        </select>
      </td>
    </tr>
    <tr>
      <td align="right" class="NoiseFieldCaptionTD" colspan="2" nowrap>
        <input class="NoiseButton" type="submit" value="Search">&nbsp; </td>
    </tr>
  </table>
</form>
</p>
<%@ include file="Footer.jsp" %>
</body>
</html>
