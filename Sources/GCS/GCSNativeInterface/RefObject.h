#ifndef REF_OBJECT_H_
#define REF_OBJECT_H_

class RefObject
{
public:
    RefObject();

    void AddRef();
    int GetRefCount();
    void Release();

protected:
    virtual ~RefObject();

private:
    int ref_count_;
};


// Inline Functions
inline RefObject::RefObject()
    : ref_count_(1)
{
}

// virtual
inline RefObject::~RefObject()
{
}

inline void RefObject::AddRef()
{
    ++ref_count_;
}

inline int RefObject::GetRefCount()
{
    return ref_count_;
}

inline void RefObject::Release()
{
    --ref_count_;
    if (ref_count_ <= 0)
    {
        delete this;
    }
}

#endif // REF_OBJECT_H_
