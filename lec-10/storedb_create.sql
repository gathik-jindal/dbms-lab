-- to run this script type: source <path to script>

create database storedb;
use storedb;

create table Stock (
    Stock_ID integer not null unique primary key,
    ShelfQty integer not null,
    StorageQty integer not null,
    ReorderThreshold integer not null,
    StockDate date not null,
    Store_ID integer not null,
    Product_ID integer not null
-- 2 foreign keys store_id and product_id go here
);

create table Store (
    Store_ID integer not null unique primary key,
    StoreName varchar(30) not null,
    Address varchar(100) not null
);

create table Product (
    Product_ID integer not null unique primary key,
    SKU integer not null unique,
    ProdName varchar(30) not null,
    Price integer not null
);

create table ShelfLocation (
    ShelfLocation_ID integer not null unique primary key,
    FloorNumber integer not null,
    AisleName varchar(30) not null,
    ShelfNumber integer not null,
    Store_ID integer not null,
    Product_ID integer not null
-- 2 foreign keys, store_id and product_id go here
);

create table Category (
    Category_ID integer not null unique primary key,
    CategoryName varchar(30) not null unique,
    CategoryDesc varchar(100)
);

create table ProductCategory (
    Relation_ID integer not null unique primary key,
    Category_ID integer,
    Product_ID integer,
    foreign key (Category_ID) references Category(Category_ID),
    foreign key (Product_ID) references Product(Product_ID)
);

create table Bill (
    Bill_ID integer not null unique primary key,
    TotalAmount integer not null,
    BillDate date not null,
    Payment_ID integer not null
-- 1 foreign key Payment_ID goes here
);

create table BillItem (
    BillItem_ID integer not null unique primary key,
    Quantity integer not null,
    TotalPrice integer not null,
    Bill_ID integer not null,
    Product_ID integer not null
-- 2 foreign keys Bill_ID and Product_ID go here
);

create table Payment (
    Payment_ID integer not null unique primary key,
    PaymentDate date not null,
    PaymentType varchar(30) not null,
    PaymentAmount integer not null,
    Bill_ID integer not null references Bill(Bill_ID)
-- 1 foreign key Bill_ID goes here
);

create table CardPayment (
    CardPayment_ID integer not null unique primary key references Payment(Payment_ID) on delete cascade,
    CardNumber integer not null,
    ExpiryDate date not null,
    AuthReferenceNumber integer not null unique
);