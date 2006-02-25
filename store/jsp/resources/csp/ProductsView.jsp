<%@ page import="store.Category,store.Product,
                 java.util.ArrayList" %>
<html>
<head>
<title>SearchResults</title>
<link rel="stylesheet" type="text/css" href="style/Style.css">
</head>
<body bgcolor="#FFFFFF" link="#000000" alink="#FF0000" vlink="#000000" text="#000000" class="NoisePageBODY">
<%@ include file="Header.jsp" %>
<p>
<form method="get">
  <font class="NoiseFormHeaderFont">Search Products</font> 
  <table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE">
    <% if(request.getAttribute("error")!=null) { %>
    <tr>
      <td colspan="2" class="NoiseErrorDataTD"><%= request.getAttribute("error") %></td>
    </tr>
    <% } %>

    <tr>
      <td class="NoiseFieldCaptionTD">Category</td> 
      <td class="NoiseDataTD">
        <select class="NoiseSelect" name="category_id">
          <option selected value="">All</option>
          <% 
            ArrayList categories = (ArrayList) request.getAttribute("categories");
            for(int i=0;i<categories.size();++i) {
          %>
          <option value="<%=((Category)categories.get(i)).id%>"><%=((Category)categories.get(i)).name%></option>
          <% } %>                        
        </select>
      </td> 
    </tr>
 
    <tr>
      <td class="NoiseFieldCaptionTD">Keyword</td> 
      <td class="NoiseDataTD"><input class="NoiseInput" maxlength="250" name="s_keyword" value="<%=request.getParameter("s_keyword")%>">&nbsp;</td> 
    </tr>
 
    <tr>
      <td colspan="2" align="right" class="NoiseFieldCaptionTD" nowrap>
        <input class="NoiseButton" type="submit" value="Search">&nbsp; </td> 
    </tr>
 
  </table>
</form>
</p>
<font class="NoiseFormHeaderFont" size="4"><b>Search
Results - <font class="NoiseFormHeaderFont" size="8"><%=request.getAttribute("ResultsFound")%></font>&nbsp;products
found.</b></font>

<table border="0" cellpadding="2" cellspacing="1" class="NoiseFormTABLE">
  <% 
  ArrayList products = (ArrayList) request.getAttribute("products");
  if(!products.isEmpty()) {
    for(int i=0;i<products.size(); ++i) {
        final Product product = (Product) products.get(i);
  %> 
  <tr>
    <td class="NoiseColumnTD" rowspan="2"><a class="NoiseDataLink" href="ProductDetails.sxx?product_id=<%=product.product_id%>"><img class="NoiseInput" src="<%=product.img%>" border="0"></a></td> 
    <td class="NoiseDataTD"><a class="NoiseDataLink" href="ProductDetails.sxx?product_id=<%=product.product_id%>"><%=product.name%></a></td> 
  </tr>
 
  <tr>
    <td class="NoiseDataTD">Price: $<%=product.price%></td> 
  </tr>
 
  <tr>
    <td class="NoiseDataTD" colspan="2">&nbsp;</td> 
  </tr>
  <%  } } else { %>
  <tr>
    <td class="NoiseDataTD" colspan="2">No records&nbsp;</td> 
  </tr>
  <% } %>
  <tr>
    <td align="middle" class="NoiseFieldCaptionTD" colspan="2" nowrap>&nbsp; 
      <% 
        int pageCount = ((Integer)request.getAttribute("pageCount")).intValue();
        int currentPage = ((Integer)request.getAttribute("currentPage")).intValue();
        String naviLink = (String) request.getAttribute("naviLink");
      %>
      <%
        if(currentPage>1) { 
      %>
        <a class="NoiseNavigatorLink" href="<%=naviLink%><%=1%>">First</a>
        <a class="NoiseNavigatorLink" href="<%=naviLink%><%=currentPage+1%>">Prev</a>
      <% } %>  
      <%
        for(int page_no=1;page_no<=pageCount; ++page_no) {
            if(page_no==currentPage)
                out.print(page_no);
            else { %>
      <a class="NoiseNavigatorLink" href="<%=naviLink%><%=page_no%>"><%=page_no%></a>
            <% } %>
      &nbsp;
        <% } %>
      of
      <%= pageCount %>&nbsp; 
      <% if(currentPage<pageCount) { %>
        <a class="NoiseNavigatorLink" href="<%=naviLink%><%=currentPage+1%>">Next</a>
        <a class="NoiseNavigatorLink" href="<%=naviLink%><%=pageCount%>">Last</a>
      <% } %>  
    </td> 
  </tr>
</table>
<%@ include file="Footer.jsp" %>
</body>
</html>
