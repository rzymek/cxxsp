protected:
    virtual const boost::any& __getAttributeImpl(const std::string& name) const = 0;
    virtual void  __setAttributeImpl(const std::string& name, const boost::any& value) = 0;
private:
