/*
 * ====================================================================
 *
 * The Apache Software License, Version 1.1
 *
 * Copyright (c) 1999-2001 The Apache Software Foundation.  All rights
 * reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * 3. The end-user documentation included with the redistribution, if
 *    any, must include the following acknowlegement:
 *       "This product includes software developed by the
 *        Apache Software Foundation (http://www.apache.org/)."
 *    Alternately, this acknowlegement may appear in the software itself,
 *    if and wherever such third-party acknowlegements normally appear.
 *
 * 4. The names "The Jakarta Project", "Tomcat", and "Apache Software
 *    Foundation" must not be used to endorse or promote products derived
 *    from this software without prior written permission. For written
 *    permission, please contact apache@apache.org.
 *
 * 5. Products derived from this software may not be called "Apache"
 *    nor may "Apache" appear in their names without prior written
 *    permission of the Apache Group.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE APACHE SOFTWARE FOUNDATION OR
 * ITS CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 * ====================================================================
 *
 */






#ifndef SERVLET_CONFIG_H
#define SERVLET_CONFIG_H

#include <string>
#include <vector>
#include <ServletContext.h>

namespace servlet {
    /**
     * 
     * A servlet configuration object used by a servlet container
     * to pass information to a servlet during initialization. 
     *
     */
    class ServletConfig {
    public:
        
    
        /**
         * Returns the name of this servlet instance.
         * The name may be provided via server administration, assigned in the 
         * web application deployment descriptor, or for an unregistered (and thus
         * unnamed) servlet instance it will be the servlet's class name.
         *
         * @return		the name of the servlet instance
         *
         *
         *
         */
        virtual std::string getServletName() const =0 ;
    
        /**
         * Returns a reference to the {@link ServletContext} in which the caller
         * is executing.
         *
         *
         * @return		a {@link ServletContext} object, used
         *			by the caller to interact with its servlet 
         *                  container
         * 
         * @see		ServletContext
         *
         */
        virtual ServletContext&  getServletContext() const =0;
        
        /**
         * Returns a <code>String</code> containing the value of the 
         * named initialization parameter, or <code>null</code> if 
         * the parameter does not exist.
         *
         * @param name	a <code>String</code> specifying the name
         *			of the initialization parameter
         *
         * @return		a <code>String</code> containing the value 
         *			of the initialization parameter
         *
         */
        virtual std::string  getInitParameter(const std::string& name) const =0;
    
    
        /**
         * Returns the names of the servlet's initialization parameters
         * as an <code>Enumeration</code> of <code>String</code> objects, 
         * or an empty <code>Enumeration</code> if the servlet has
         * no initialization parameters.
         *
         * @return		an <code>Enumeration</code> of <code>String</code> 
         *			objects containing the names of the servlet's 
         *			initialization parameters
         *
         *
         *
         */
        virtual std::auto_ptr< std::vector<std::string> > getInitParameterNames() const =0;
    };

}//namespace
#endif/*SERVLET_CONFIG_H*/
