<%@page import="store.Product" session="false" %>
<% Product p = (Product) request.getAttribute("product"); %>
<html>
<head>
<title>ProductDetail</title>
<link rel="stylesheet" type="text/css" href="style/Style.css">
</head>
<body bgcolor="#ffffff" link="#000000" alink="#ff0000" vlink="#000000" text="#000000" class="NoisePageBODY">
  <font class="NoiseFormHeaderFont">Product Detail</font>
  <table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE">
    <% if(request.getAttribute("error") != null) { %>
    <tr>
      <td colspan="2" class="NoiseErrorDataTD"><%= request.getAttribute("error") %></td>
    </tr>
    <% } %>
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

</body>
</html>
