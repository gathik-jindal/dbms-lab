alter table Stock
    add foreign key (Store_ID) references Store(Store_ID),
    add foreign key (Product_ID) references Product(Product_ID);

alter table ShelfLocation
    add foreign key (Store_ID) references Store(Store_ID),
    add foreign key (Product_ID) references Product(Product_ID);

alter table ProductCategory
    add foreign key (Category_ID) references Category(Category_ID),
    add foreign key (Product_ID) references Product(Product_ID);

alter table Bill
    add foreign key (Payment_ID) references Payment(Payment_ID);

alter table BillItem
    add foreign key (Bill_ID) references Bill(Bill_ID) on delete cascade,
    add foreign key (Product_ID) references Product(Product_ID);