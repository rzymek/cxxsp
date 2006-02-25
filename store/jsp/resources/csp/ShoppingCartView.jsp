<%@ page import="store.cart_item,
                 java.util.ArrayList,
                 store.cart_item"%>
<html>
<head>
    <title>ShoppingCart</title>
    <link rel="stylesheet" type="text/css" href="style/Style.css">
</head>
<body bgcolor="#FFFFFF" link="#000000" alink="#FF0000" vlink="#000000" text="#000000" class="NoisePageBODY">
<%@ include file="Header.jsp" %>
<% 
    ArrayList cart = (ArrayList) request.getAttribute("cart");
    if(!cart.isEmpty()) {
%>
    <font class="NoiseFormHeaderFont">Shopping Cart Items</font>
      <form method="post" action="ShoppingCart.sxx">
        <input type="hidden" name="form" value="cart_edit">
        <font class="NoiseFormHeaderFont">&nbsp;&nbsp; </font>
        <table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE">
          <% if(request.getAttribute("error")!=null) { %>
          <tr>
            <td class="NoiseErrorDataTD" colspan="5"><%= request.getAttribute("error") %></td>
          </tr>
          <% } %>
          <tr>
            <td class="NoiseColumnTD" nowrap>Product&nbsp;</td>
            <td class="NoiseColumnTD" nowrap align="right">Price</td>
            <td class="NoiseColumnTD" nowrap align="right">Quantity&nbsp;</td>
            <td class="NoiseColumnTD" nowrap align="right">Total</td>
            <td class="NoiseColumnTD" nowrap>Delete&nbsp;</td>
          </tr>

          <% for(int i=0;i<cart.size(); ++i) { %>
          <tr>  
            <td class="NoiseDataTD">
                <input type="hidden" name="product_id_<%=i%>" value="<%=((cart_item)cart.get(i)).product.product_id%>">
                <%= ((cart_item)cart.get(i)).product.name %> &nbsp;
                </td>
            <td class="NoiseDataTD" align="right">$<%= ((cart_item)cart.get(i)).product.price %></td>
            <td class="NoiseDataTD" align="right">
                <input class="NoiseInput" maxlength="7" name="quantity_<%=i%>" size="7"
                    value="<%= ((cart_item)cart.get(i)).quantity %>">
                &nbsp;
            </td>
            <td class="NoiseDataTD" align="right">$<%=((cart_item)cart.get(i)).product.price * ((cart_item)cart.get(i)).quantity %></td>
            <td class="NoiseDataTD">
            <input class="NoiseInput" name="delete_<%=i%>" type="checkbox" value="1" >&nbsp;</td>
          </tr>
          <% } if(cart.size()==0) { %>
          <tr>
            <td class="NoiseDataTD" colspan="5">No records&nbsp;</td>
          </tr>
          <% } %>
          <tr>
            <td align="right" class="NoiseFieldCaptionTD" colspan="5" nowrap>&nbsp;
              <input class="NoiseButton" type="submit" value="Submit">&nbsp; </td>
          </tr>

        </table>

      </form>
    
    
      <table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE">
        <tr>
          <td class="NoiseFieldCaptionTD"><b>Total</b></td>
          <td class="NoiseDataTD" align="right">
            $<%= request.getAttribute("total") %>
          </td>
        </tr>

      </table>

    <br>
    
    <form method="post">
      <input type="hidden" name="form" value="purchase">
      <input class="NoiseButton" type="submit" value="Click here to Purchase">
    </form>
    
<% } else { %>
    <br>
    <table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE">
      <tr>
        <td class="NoiseFieldCaptionTD">Shopping Cart is empty</td>
      </tr>
    </table>
        
<% } %>        

<%@ include file="Footer.jsp" %>

</body>
</html>
