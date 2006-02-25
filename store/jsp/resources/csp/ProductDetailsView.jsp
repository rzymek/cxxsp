<%@page import="store.Product" %>
<% Product p = (Product) request.getAttribute("product"); %>
<html>
<head>
<title>ProductDetail</title>
<link rel="stylesheet" type="text/css" href="style/Style.css">
</head>
<body bgcolor="#ffffff" link="#000000" alink="#ff0000" vlink="#000000" text="#000000" class="NoisePageBODY">
<%@ include file="Header.jsp" %>
  <font class="NoiseFormHeaderFont">Product Detail</font>
  <table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE">
    <tr>
      <td rowspan="3" class="NoiseFieldCaptionTD"><img class="NoiseInput" src="<%= p.img %>"></td>
      <td class="NoiseDataTD"><%= p.name %></td>
    </tr>

    <tr>
      <td class="NoiseDataTD">Price: $<%= p.price %></td>
    </tr>

    <tr> 
      <td class="NoiseDataTD"><%= p.desc %></td>
    </tr>

  </table>
<!-- BEGIN Record add_to_cart -->
<form method="post">
  <table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE" width="200">
    <% if(request.getAttribute("error")!=null) { %>
    <tr>
      <td colspan="2" class="NoiseErrorDataTD"><%= request.getAttribute("error") %></td>
    </tr>
    <% } %>
    <!-- END Error -->
    <tr>
      <td class="NoiseFieldCaptionTD" align="right">Quantity&nbsp;</td>
      <td class="NoiseDataTD"><input class="NoiseInput" maxlength="7" name="quantity" size="7" value="1">&nbsp;</td>
    </tr>

    <tr>
      <td align="right" class="NoiseFieldCaptionTD" colspan="2" nowrap>
        <input class="NoiseButton" type="submit" value="Add to Shopping Cart"></td>
    </tr>

  </table>
 <input name="product_id" type="hidden" value="<%= request.getParameter("product_id") %>">
</form>
<!-- END Record add_to_cart -->
<%@ include file="Footer.jsp" %>

</body>
</html>
