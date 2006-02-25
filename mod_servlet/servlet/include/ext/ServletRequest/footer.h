   
namespace servlet {    
    
    template<typename T>
    T ServletRequest::getAttribute(const std::string& name) const {    
        const boost::any& value = __getAttributeImpl(name);
        if(value.empty())
            return T();
        return boost::any_cast<T>(value);
    }

    template<typename T>
    void ServletRequest::setAttribute(const std::string& name, const T& t) {
        __setAttributeImpl(name, boost::any(t));
    }
    
}
