# Index and desription for the current directory's cpp file.

> [class_const_member_function.cc](./class_const_member_function.cc)
> > NOTES:
> > 当类对象的引用作为参数的时候，它所访问的成员函数必须为const，否则不能通过编译。

> [class_member_deconstructor.cc](./class_member_deconstructor.cc)  
> > NOTES:
> > 当A类作为B类的成员变量的时候，析构的时候先调用B的析构函数再调用A的析构函数。

> [empty_vector_front_back.cc](./empty_vector_front_back.cc)
> > NOTES:
> > 声明一个vecotr对象，直接调用front或者back成员函数，程序会coredump。

> [hared_ptr_change_pointer.cc](./hared_ptr_change_pointer.cc)
> > NOTES:
> > 当智能指针更换指向的时候，之前的所指向的对象会被析构，可以通过直接将智能指针赋值为空析构指向的对象。

> [shared_ptr_use_count.cc](./shared_ptr_use_count.cc)
> > NOTES:
> > 当智能指针作为参数的时候，如果是值拷贝,值拷贝的临时对象也会增加引用。

> [shared_ptr_valid.cc](./shared_ptr_valid.cc)
> > NOTES:
> > 智能指针的判断可以通过get()获取裸指针进行判断，也可以直接判断智能指针对象本身，因为重载了bool()操作符。

> [shared_ptr_release.cc](./shared_ptr_release.cc)
> > NOTES:
> > 将智能指针赋值为nullptr，如只有它引用这个对象，那么这个对象将会析构。

> [linux_file_op_mkdir.cc](./linux_file_op_mkdir.cc)
> > NOTES:
> > 使用了C++11的正则表达式来切分路径，然后重路径并创建不存在的目录。(bug:同路径下已存在同名文件则会创建失败)
