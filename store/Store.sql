# 
# CREATE DATABASE
# 
CREATE DATABASE IF NOT EXISTS internet;
USE internet;


# 
# DROP TABLES
# 
DROP TABLE IF EXISTS ages;
DROP TABLE IF EXISTS articles;
DROP TABLE IF EXISTS countries;
DROP TABLE IF EXISTS directory_categories;
DROP TABLE IF EXISTS directory_items;
DROP TABLE IF EXISTS educations;
DROP TABLE IF EXISTS events;
DROP TABLE IF EXISTS event_categories;
DROP TABLE IF EXISTS forum_messages;
DROP TABLE IF EXISTS genders;
DROP TABLE IF EXISTS incomes;
DROP TABLE IF EXISTS languages;
DROP TABLE IF EXISTS links;
DROP TABLE IF EXISTS link_categories;
DROP TABLE IF EXISTS news;
DROP TABLE IF EXISTS officers;
DROP TABLE IF EXISTS smiles;
DROP TABLE IF EXISTS states;
DROP TABLE IF EXISTS users;
DROP TABLE IF EXISTS user_groups;

DROP TABLE IF EXISTS store_categories;
DROP TABLE IF EXISTS store_editorial_categories;
DROP TABLE IF EXISTS store_editorial_products;
DROP TABLE IF EXISTS store_order_items;
DROP TABLE IF EXISTS store_orders;
DROP TABLE IF EXISTS store_products;
DROP TABLE IF EXISTS store_shopping_cart_items;
DROP TABLE IF EXISTS store_shopping_carts;
DROP TABLE IF EXISTS store_statuses;



# 
# CREATE TABLES
# 

CREATE TABLE users (
    user_id          integer     auto_increment primary key,
    user_login       varchar(50) null,
    user_password    varchar(50) null,
    first_name       varchar(50) null,
    last_name        varchar(50) null,
    title            varchar(50) null,
    group_id         integer     null,
    phone_home       varchar(50) null,
    phone_work       varchar(50) null,
    phone_day        varchar(50) null,
    phone_cell       varchar(50) null,
    phone_evening    varchar(50) null,
    fax              varchar(50) null,
    email            varchar(50) null,
    notes            text        null,
    card_number      varchar(50) null,
    card_expire_date varchar(50) null,
    country_id       integer     null,
    state_id         integer     null,
    city             varchar(50) null,
    zip              varchar(50) null,
    address1         varchar(50) null,
    address2         varchar(50) null,
    address3         varchar(50) null,
    date_add         datetime    null,
    date_last_login  datetime    null,
    ip_add           varchar(50) null,
    ip_update        varchar(50) null,
    language_id      integer     null,
    image_url        varchar(50) null,
    age_id           integer     null,
    gender_id        integer     null,
    education_id     integer     null,
    income_id        integer     null,
    user_SSN         varchar(50) NULL
);

#CREATE TABLE user_groups (
#    group_id   integer     primary key,
#    group_name varchar(50) null
#);

CREATE TABLE countries (
    country_id   integer     auto_increment primary key,
    country_name varchar(50) null
);


###
# CREATE STORE TABLES
###
CREATE TABLE store_categories (
	category_id 	integer     auto_increment primary key,
	category_name   varchar(50)  NOT NULL 
) ;


CREATE TABLE store_editorial_categories (
	category_id 	integer     auto_increment primary key,
	category_name 	varchar(50)  NULL 
);


CREATE TABLE store_editorial_products (
	article_id		integer     auto_increment primary key,
	editorial_cat_id	integer	 NULL ,
	article_title		varchar(200) NULL ,
	article_desc		text    NULL ,
	product_id 		integer NULL 
);


CREATE TABLE store_order_items (
	order_item_id		integer     auto_increment primary key,
	order_id 		integer NULL ,
	product_id		integer NULL ,
	quantity 		integer NULL ,
	price 			real NULL 
);


CREATE TABLE store_orders (
	order_id	 	integer     auto_increment primary key,
	user_id 	 	int NULL ,
	order_date   		datetime NULL,
	order_status_id 	integer NOT NULL,
	total 			real NULL 
);


CREATE TABLE store_products (
	product_id 		integer     auto_increment primary key,
	category_id 		integer NOT NULL,
	product_name	 	varchar(255)  NOT NULL,
	price 			float NOT NULL,
	image_url		varchar(100)  NULL,
	description 		text  NULL,
	is_recommended 		integer  NULL 
);


CREATE TABLE store_shopping_cart_items (
	shopping_cart_item_id	integer     auto_increment primary key,
	shopping_cart_id	integer NULL ,
	product_id		integer NULL ,
	quantity		integer NULL 
);


CREATE TABLE store_shopping_carts (
	shopping_cart_id	integer     auto_increment primary key,
	date_add		datetime NULL 
);


CREATE TABLE store_statuses (
	status_id	integer     auto_increment primary key,
	status_name	varchar(50) NULL 
);

# 
# TABLE INSERT STATEMENTS
# 
#INSERT INTO user_groups(group_id,group_name) VALUES(2,'Admin');

INSERT INTO users(user_id,user_login,user_password,first_name,last_name,title,group_id,phone_home,phone_work,phone_day,phone_cell,phone_evening,fax,email,notes,card_number,card_expire_date,country_id,state_id,city,zip,address1,address2,address3,date_add,date_last_login,ip_add,ip_update,language_id,image_url,age_id,gender_id,education_id,income_id)
      VALUES(1,'guest','guest','Guest','Member',null,1,null,null,null,null,null,null,'guest@nowhere.com',NULL,null,null,249,2,'South San Francisco','94080',null,null,null,'2000-11-24 17:41:00','2000-11-24 17:41:00','195.24.149.53',null,null,null,null,1,null,null);
INSERT INTO users(user_id,user_login,user_password,first_name,last_name,title,group_id,phone_home,phone_work,phone_day,phone_cell,phone_evening,fax,email,notes,card_number,card_expire_date,country_id,state_id,city,zip,address1,address2,address3,date_add,date_last_login,ip_add,ip_update,language_id,image_url,age_id,gender_id,education_id,income_id)
      VALUES(2,'admin','admin','Admin','User',null,2,null,null,null,null,null,null,'admin@nowhere.com',NULL,null,null,249,2,'South San Francisco','94080',null,null,null,'2000-11-24 17:41:00','2000-12-26 17:41:00','195.24.149.53',null,null,null,null,1,null,null);

INSERT INTO countries(country_id,country_name) VALUES(1,'Afghanistan');
INSERT INTO countries(country_id,country_name) VALUES(2,'Albania');
INSERT INTO countries(country_id,country_name) VALUES(3,'Algeria');
INSERT INTO countries(country_id,country_name) VALUES(4,'American Samoa');
INSERT INTO countries(country_id,country_name) VALUES(5,'Andorra');
INSERT INTO countries(country_id,country_name) VALUES(6,'Angola');
INSERT INTO countries(country_id,country_name) VALUES(7,'Anguilla');
INSERT INTO countries(country_id,country_name) VALUES(8,'Antarctica');
INSERT INTO countries(country_id,country_name) VALUES(9,'Antigua and Barbuda');
INSERT INTO countries(country_id,country_name) VALUES(10,'Argentina');
INSERT INTO countries(country_id,country_name) VALUES(11,'Armenia');
INSERT INTO countries(country_id,country_name) VALUES(12,'Aruba');
INSERT INTO countries(country_id,country_name) VALUES(13,'Ashmore/Cartier Islands');
INSERT INTO countries(country_id,country_name) VALUES(14,'Australia');
INSERT INTO countries(country_id,country_name) VALUES(15,'Austria');
INSERT INTO countries(country_id,country_name) VALUES(16,'Azerbaijan');
INSERT INTO countries(country_id,country_name) VALUES(17,'Bahamas, The');
INSERT INTO countries(country_id,country_name) VALUES(18,'Bahrain');
INSERT INTO countries(country_id,country_name) VALUES(19,'Baker Island');
INSERT INTO countries(country_id,country_name) VALUES(20,'Bangladesh');
INSERT INTO countries(country_id,country_name) VALUES(21,'Barbados');
INSERT INTO countries(country_id,country_name) VALUES(22,'Bassas da India');
INSERT INTO countries(country_id,country_name) VALUES(23,'Belarus');
INSERT INTO countries(country_id,country_name) VALUES(24,'Belgium');
INSERT INTO countries(country_id,country_name) VALUES(25,'Belize');
INSERT INTO countries(country_id,country_name) VALUES(26,'Benin');
INSERT INTO countries(country_id,country_name) VALUES(27,'Bermuda');
INSERT INTO countries(country_id,country_name) VALUES(28,'Bhutan');
INSERT INTO countries(country_id,country_name) VALUES(29,'Bolivia');
INSERT INTO countries(country_id,country_name) VALUES(30,'Bosnia and Herzegovina');
INSERT INTO countries(country_id,country_name) VALUES(31,'Botswana');
INSERT INTO countries(country_id,country_name) VALUES(32,'Bouvet Island');
INSERT INTO countries(country_id,country_name) VALUES(33,'Brazil');
INSERT INTO countries(country_id,country_name) VALUES(34,'British Indian Ocean Territory');
INSERT INTO countries(country_id,country_name) VALUES(35,'British Virgin Islands');
INSERT INTO countries(country_id,country_name) VALUES(36,'Brunei');
INSERT INTO countries(country_id,country_name) VALUES(37,'Bulgaria');
INSERT INTO countries(country_id,country_name) VALUES(38,'Burkina Faso');
INSERT INTO countries(country_id,country_name) VALUES(39,'Burma');
INSERT INTO countries(country_id,country_name) VALUES(40,'Burundi');
INSERT INTO countries(country_id,country_name) VALUES(41,'Cambodia');
INSERT INTO countries(country_id,country_name) VALUES(42,'Cameroon');
INSERT INTO countries(country_id,country_name) VALUES(43,'Canada');
INSERT INTO countries(country_id,country_name) VALUES(44,'Cape Verde');
INSERT INTO countries(country_id,country_name) VALUES(45,'Cayman Islands');
INSERT INTO countries(country_id,country_name) VALUES(46,'Central African Republic');
INSERT INTO countries(country_id,country_name) VALUES(47,'Chad');
INSERT INTO countries(country_id,country_name) VALUES(48,'Chile');
INSERT INTO countries(country_id,country_name) VALUES(49,'China');
INSERT INTO countries(country_id,country_name) VALUES(50,'Christmas Island');
INSERT INTO countries(country_id,country_name) VALUES(51,'Clipperton Island');
INSERT INTO countries(country_id,country_name) VALUES(52,'Cocos (Keeling) Islands');
INSERT INTO countries(country_id,country_name) VALUES(53,'Colombia');
INSERT INTO countries(country_id,country_name) VALUES(54,'Comoros');
INSERT INTO countries(country_id,country_name) VALUES(55,'Congo, Democratic Republic of the');
INSERT INTO countries(country_id,country_name) VALUES(56,'Congo, Republic of the');
INSERT INTO countries(country_id,country_name) VALUES(57,'Cook Islands');
INSERT INTO countries(country_id,country_name) VALUES(58,'Coral Sea Islands');
INSERT INTO countries(country_id,country_name) VALUES(59,'Costa Rica');
INSERT INTO countries(country_id,country_name) VALUES(60,'Cote d\'Ivoire');
INSERT INTO countries(country_id,country_name) VALUES(61,'Croatia');
INSERT INTO countries(country_id,country_name) VALUES(62,'Cuba');
INSERT INTO countries(country_id,country_name) VALUES(63,'Cyprus');
INSERT INTO countries(country_id,country_name) VALUES(64,'Czech Republic');
INSERT INTO countries(country_id,country_name) VALUES(65,'Denmark');
INSERT INTO countries(country_id,country_name) VALUES(66,'Djibouti');
INSERT INTO countries(country_id,country_name) VALUES(67,'Dominica');
INSERT INTO countries(country_id,country_name) VALUES(68,'Dominican Republic');
INSERT INTO countries(country_id,country_name) VALUES(69,'East Timor');
INSERT INTO countries(country_id,country_name) VALUES(70,'Ecuador');
INSERT INTO countries(country_id,country_name) VALUES(71,'Egypt');
INSERT INTO countries(country_id,country_name) VALUES(72,'El Salvador');
INSERT INTO countries(country_id,country_name) VALUES(73,'Equatorial Guinea');
INSERT INTO countries(country_id,country_name) VALUES(74,'Eritrea');
INSERT INTO countries(country_id,country_name) VALUES(75,'Estonia');
INSERT INTO countries(country_id,country_name) VALUES(76,'Ethiopia');
INSERT INTO countries(country_id,country_name) VALUES(77,'Europa Island');
INSERT INTO countries(country_id,country_name) VALUES(78,'Falkland Islands (Islas Malvinas)');
INSERT INTO countries(country_id,country_name) VALUES(79,'Faroe Islands');
INSERT INTO countries(country_id,country_name) VALUES(80,'Fiji');
INSERT INTO countries(country_id,country_name) VALUES(81,'Finland');
INSERT INTO countries(country_id,country_name) VALUES(82,'France');
INSERT INTO countries(country_id,country_name) VALUES(83,'French Guiana');
INSERT INTO countries(country_id,country_name) VALUES(84,'French Polynesia');
INSERT INTO countries(country_id,country_name) VALUES(85,'French Southern and Antarctic Lands');
INSERT INTO countries(country_id,country_name) VALUES(86,'Gabon');
INSERT INTO countries(country_id,country_name) VALUES(87,'Gambia, The');
INSERT INTO countries(country_id,country_name) VALUES(88,'Gaza Strip');
INSERT INTO countries(country_id,country_name) VALUES(89,'Georgia');
INSERT INTO countries(country_id,country_name) VALUES(90,'Germany');
INSERT INTO countries(country_id,country_name) VALUES(91,'Ghana');
INSERT INTO countries(country_id,country_name) VALUES(92,'Gibraltar');
INSERT INTO countries(country_id,country_name) VALUES(93,'Glorioso Islands');
INSERT INTO countries(country_id,country_name) VALUES(94,'Greece');
INSERT INTO countries(country_id,country_name) VALUES(95,'Greenland');
INSERT INTO countries(country_id,country_name) VALUES(96,'Grenada');
INSERT INTO countries(country_id,country_name) VALUES(97,'Guadeloupe');
INSERT INTO countries(country_id,country_name) VALUES(98,'Guam');
INSERT INTO countries(country_id,country_name) VALUES(99,'Guatemala');
INSERT INTO countries(country_id,country_name) VALUES(100,'Guernsey');
INSERT INTO countries(country_id,country_name) VALUES(101,'Guinea');
INSERT INTO countries(country_id,country_name) VALUES(102,'Guinea-Bissau');
INSERT INTO countries(country_id,country_name) VALUES(103,'Guyana');
INSERT INTO countries(country_id,country_name) VALUES(104,'Haiti');
INSERT INTO countries(country_id,country_name) VALUES(105,'Heard Island and McDonald Islands');
INSERT INTO countries(country_id,country_name) VALUES(106,'Holy See (Vatican City)');
INSERT INTO countries(country_id,country_name) VALUES(107,'Honduras');
INSERT INTO countries(country_id,country_name) VALUES(108,'Hong Kong');
INSERT INTO countries(country_id,country_name) VALUES(109,'Howland Island');
INSERT INTO countries(country_id,country_name) VALUES(110,'Hungary');
INSERT INTO countries(country_id,country_name) VALUES(111,'Iceland');
INSERT INTO countries(country_id,country_name) VALUES(112,'India');
INSERT INTO countries(country_id,country_name) VALUES(113,'Indonesia');
INSERT INTO countries(country_id,country_name) VALUES(114,'Iran');
INSERT INTO countries(country_id,country_name) VALUES(115,'Iraq');
INSERT INTO countries(country_id,country_name) VALUES(116,'Ireland');
INSERT INTO countries(country_id,country_name) VALUES(117,'Israel');
INSERT INTO countries(country_id,country_name) VALUES(118,'Italy');
INSERT INTO countries(country_id,country_name) VALUES(119,'Jamaica');
INSERT INTO countries(country_id,country_name) VALUES(120,'Jan Mayen');
INSERT INTO countries(country_id,country_name) VALUES(121,'Japan');
INSERT INTO countries(country_id,country_name) VALUES(122,'Jarvis Island');
INSERT INTO countries(country_id,country_name) VALUES(123,'Jersey');
INSERT INTO countries(country_id,country_name) VALUES(124,'Johnston Atoll');
INSERT INTO countries(country_id,country_name) VALUES(125,'Jordan');
INSERT INTO countries(country_id,country_name) VALUES(126,'Juan de Nova Island');
INSERT INTO countries(country_id,country_name) VALUES(127,'Kazakhstan');
INSERT INTO countries(country_id,country_name) VALUES(128,'Kenya');
INSERT INTO countries(country_id,country_name) VALUES(129,'Kingman Reef');
INSERT INTO countries(country_id,country_name) VALUES(130,'Kiribati');
INSERT INTO countries(country_id,country_name) VALUES(131,'Korea, North');
INSERT INTO countries(country_id,country_name) VALUES(132,'Korea, South');
INSERT INTO countries(country_id,country_name) VALUES(133,'Kuwait');
INSERT INTO countries(country_id,country_name) VALUES(134,'Kyrgyzstan');
INSERT INTO countries(country_id,country_name) VALUES(135,'Laos');
INSERT INTO countries(country_id,country_name) VALUES(136,'Latvia');
INSERT INTO countries(country_id,country_name) VALUES(137,'Lebanon');
INSERT INTO countries(country_id,country_name) VALUES(138,'Lesotho');
INSERT INTO countries(country_id,country_name) VALUES(139,'Liberia');
INSERT INTO countries(country_id,country_name) VALUES(140,'Libya');
INSERT INTO countries(country_id,country_name) VALUES(141,'Liechtenstein');
INSERT INTO countries(country_id,country_name) VALUES(142,'Lithuania');
INSERT INTO countries(country_id,country_name) VALUES(143,'Luxembourg');
INSERT INTO countries(country_id,country_name) VALUES(144,'Macau');
INSERT INTO countries(country_id,country_name) VALUES(145,'Macedonia');
INSERT INTO countries(country_id,country_name) VALUES(146,'Madagascar');
INSERT INTO countries(country_id,country_name) VALUES(147,'Malawi');
INSERT INTO countries(country_id,country_name) VALUES(148,'Malaysia');
INSERT INTO countries(country_id,country_name) VALUES(149,'Maldives');
INSERT INTO countries(country_id,country_name) VALUES(150,'Mali');
INSERT INTO countries(country_id,country_name) VALUES(151,'Malta');
INSERT INTO countries(country_id,country_name) VALUES(152,'Man, Isle of');
INSERT INTO countries(country_id,country_name) VALUES(153,'Marshall Islands');
INSERT INTO countries(country_id,country_name) VALUES(154,'Martinique');
INSERT INTO countries(country_id,country_name) VALUES(155,'Mauritania');
INSERT INTO countries(country_id,country_name) VALUES(156,'Mauritius');
INSERT INTO countries(country_id,country_name) VALUES(157,'Mayotte');
INSERT INTO countries(country_id,country_name) VALUES(158,'Mexico');
INSERT INTO countries(country_id,country_name) VALUES(159,'Micronesia, Federated States of');
INSERT INTO countries(country_id,country_name) VALUES(160,'Midway Islands');
INSERT INTO countries(country_id,country_name) VALUES(161,'Moldova');
INSERT INTO countries(country_id,country_name) VALUES(162,'Monaco');
INSERT INTO countries(country_id,country_name) VALUES(163,'Mongolia');
INSERT INTO countries(country_id,country_name) VALUES(164,'Montserrat');
INSERT INTO countries(country_id,country_name) VALUES(165,'Morocco');
INSERT INTO countries(country_id,country_name) VALUES(166,'Mozambique');
INSERT INTO countries(country_id,country_name) VALUES(167,'Namibia');
INSERT INTO countries(country_id,country_name) VALUES(168,'Nauru');
INSERT INTO countries(country_id,country_name) VALUES(169,'Navassa Island');
INSERT INTO countries(country_id,country_name) VALUES(170,'Nepal');
INSERT INTO countries(country_id,country_name) VALUES(171,'Netherlands');
INSERT INTO countries(country_id,country_name) VALUES(172,'Netherlands Antilles');
INSERT INTO countries(country_id,country_name) VALUES(173,'New Caledonia');
INSERT INTO countries(country_id,country_name) VALUES(174,'New Zealand');
INSERT INTO countries(country_id,country_name) VALUES(175,'Nicaragua');
INSERT INTO countries(country_id,country_name) VALUES(176,'Niger');
INSERT INTO countries(country_id,country_name) VALUES(177,'Nigeria');
INSERT INTO countries(country_id,country_name) VALUES(178,'Niue');
INSERT INTO countries(country_id,country_name) VALUES(179,'Norfolk Island');
INSERT INTO countries(country_id,country_name) VALUES(180,'Northern Mariana Islands');
INSERT INTO countries(country_id,country_name) VALUES(181,'Norway');
INSERT INTO countries(country_id,country_name) VALUES(182,'Oman');
INSERT INTO countries(country_id,country_name) VALUES(183,'Pakistan');
INSERT INTO countries(country_id,country_name) VALUES(184,'Palau');
INSERT INTO countries(country_id,country_name) VALUES(185,'Palmyra Atoll');
INSERT INTO countries(country_id,country_name) VALUES(186,'Panama');
INSERT INTO countries(country_id,country_name) VALUES(187,'Papua New Guinea');
INSERT INTO countries(country_id,country_name) VALUES(188,'Paracel Islands');
INSERT INTO countries(country_id,country_name) VALUES(189,'Paraguay');
INSERT INTO countries(country_id,country_name) VALUES(190,'Peru');
INSERT INTO countries(country_id,country_name) VALUES(191,'Philippines');
INSERT INTO countries(country_id,country_name) VALUES(192,'Pitcairn Islands');
INSERT INTO countries(country_id,country_name) VALUES(193,'Poland');
INSERT INTO countries(country_id,country_name) VALUES(194,'Portugal');
INSERT INTO countries(country_id,country_name) VALUES(195,'Puerto Rico');
INSERT INTO countries(country_id,country_name) VALUES(196,'Qatar');
INSERT INTO countries(country_id,country_name) VALUES(197,'Reunion');
INSERT INTO countries(country_id,country_name) VALUES(198,'Romania');
INSERT INTO countries(country_id,country_name) VALUES(199,'Russia');
INSERT INTO countries(country_id,country_name) VALUES(200,'Rwanda');
INSERT INTO countries(country_id,country_name) VALUES(201,'Saint Helena');
INSERT INTO countries(country_id,country_name) VALUES(202,'Saint Kitts and Nevis');
INSERT INTO countries(country_id,country_name) VALUES(203,'Saint Lucia');
INSERT INTO countries(country_id,country_name) VALUES(204,'Saint Pierre and Miquelon');
INSERT INTO countries(country_id,country_name) VALUES(205,'Saint Vincent and the Grenadines');
INSERT INTO countries(country_id,country_name) VALUES(206,'Samoa');
INSERT INTO countries(country_id,country_name) VALUES(207,'San Marino');
INSERT INTO countries(country_id,country_name) VALUES(208,'Sao Tome and Principe');
INSERT INTO countries(country_id,country_name) VALUES(209,'Saudi Arabia');
INSERT INTO countries(country_id,country_name) VALUES(210,'Senegal');
INSERT INTO countries(country_id,country_name) VALUES(211,'Serbia and Montenegro');
INSERT INTO countries(country_id,country_name) VALUES(212,'Seychelles');
INSERT INTO countries(country_id,country_name) VALUES(213,'Sierra Leone');
INSERT INTO countries(country_id,country_name) VALUES(214,'Singapore');
INSERT INTO countries(country_id,country_name) VALUES(215,'Slovakia');
INSERT INTO countries(country_id,country_name) VALUES(216,'Slovenia');
INSERT INTO countries(country_id,country_name) VALUES(217,'Solomon Islands');
INSERT INTO countries(country_id,country_name) VALUES(218,'Somalia');
INSERT INTO countries(country_id,country_name) VALUES(219,'South Africa');
INSERT INTO countries(country_id,country_name) VALUES(220,'South Georgia/South Sandwich Islands');
INSERT INTO countries(country_id,country_name) VALUES(221,'Spain');
INSERT INTO countries(country_id,country_name) VALUES(222,'Spratly Islands');
INSERT INTO countries(country_id,country_name) VALUES(223,'Sri Lanka');
INSERT INTO countries(country_id,country_name) VALUES(224,'Sudan');
INSERT INTO countries(country_id,country_name) VALUES(225,'Suriname');
INSERT INTO countries(country_id,country_name) VALUES(226,'Svalbard');
INSERT INTO countries(country_id,country_name) VALUES(227,'Swaziland');
INSERT INTO countries(country_id,country_name) VALUES(228,'Sweden');
INSERT INTO countries(country_id,country_name) VALUES(229,'Switzerland');
INSERT INTO countries(country_id,country_name) VALUES(230,'Syria');
INSERT INTO countries(country_id,country_name) VALUES(231,'Taiwan');
INSERT INTO countries(country_id,country_name) VALUES(232,'Tajikistan');
INSERT INTO countries(country_id,country_name) VALUES(233,'Tanzania');
INSERT INTO countries(country_id,country_name) VALUES(234,'Thailand');
INSERT INTO countries(country_id,country_name) VALUES(235,'Togo');
INSERT INTO countries(country_id,country_name) VALUES(236,'Tokelau');
INSERT INTO countries(country_id,country_name) VALUES(237,'Tonga');
INSERT INTO countries(country_id,country_name) VALUES(238,'Trinidad and Tobago');
INSERT INTO countries(country_id,country_name) VALUES(239,'Tromelin Island');
INSERT INTO countries(country_id,country_name) VALUES(240,'Tunisia');
INSERT INTO countries(country_id,country_name) VALUES(241,'Turkey');
INSERT INTO countries(country_id,country_name) VALUES(242,'Turkmenistan');
INSERT INTO countries(country_id,country_name) VALUES(243,'Turks and Caicos Islands');
INSERT INTO countries(country_id,country_name) VALUES(244,'Tuvalu');
INSERT INTO countries(country_id,country_name) VALUES(245,'Uganda');
INSERT INTO countries(country_id,country_name) VALUES(246,'Ukraine');
INSERT INTO countries(country_id,country_name) VALUES(247,'United Arab Emirates');
INSERT INTO countries(country_id,country_name) VALUES(248,'United Kingdom');
INSERT INTO countries(country_id,country_name) VALUES(249,'United States');
INSERT INTO countries(country_id,country_name) VALUES(250,'US Minor Outlying Islands');
INSERT INTO countries(country_id,country_name) VALUES(251,'Uruguay');
INSERT INTO countries(country_id,country_name) VALUES(252,'Uzbekistan');
INSERT INTO countries(country_id,country_name) VALUES(253,'Vanuatu');
INSERT INTO countries(country_id,country_name) VALUES(254,'Venezuela');
INSERT INTO countries(country_id,country_name) VALUES(255,'Vietnam');
INSERT INTO countries(country_id,country_name) VALUES(256,'Virgin Islands');
INSERT INTO countries(country_id,country_name) VALUES(257,'Wake Island');
INSERT INTO countries(country_id,country_name) VALUES(258,'Wallis and Futuna');
INSERT INTO countries(country_id,country_name) VALUES(259,'West Bank');
INSERT INTO countries(country_id,country_name) VALUES(260,'Western Sahara');
INSERT INTO countries(country_id,country_name) VALUES(261,'Yemen');
INSERT INTO countries(country_id,country_name) VALUES(262,'Zambia');
INSERT INTO countries(country_id,country_name) VALUES(263,'Zimbabwe');

#########

INSERT INTO store_categories (category_id,category_name) VALUES (1,'Programming');
INSERT INTO store_categories (category_id,category_name) VALUES (2,'Databases');
INSERT INTO store_categories (category_id,category_name) VALUES (3,'HTML & Web design');


INSERT INTO store_editorial_categories (category_id,category_name) VALUES (1,'What We\'re Reading');
INSERT INTO store_editorial_categories (category_id,category_name) VALUES (2,'New & Notable');
INSERT INTO store_editorial_categories (category_id,category_name) VALUES (3,'This Week\'s Featured Books');
INSERT INTO store_editorial_categories (category_id,category_name) VALUES (4,'General');


INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (1,2,'Web Database Development',39.99,'images/books/0735609667.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (2,1,'Programming Perl',39.96,'images/books/0596000278.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (3,1,'Perl and CGI for the World Wide Web',15.19,'images/books/020135358X.gif',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (4,2,'MySQL',39.99,'images/books/0735709211.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (5,1,'PHP and MySQL Web Development',39.99,'images/books/0672317842.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (6,1,'MySQL & PHP From Scratch',23.99,'images/books/0789724405.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (7,2,'MySQL and mSQL',27.96,'images/books/1565924347.jpg',NULL,1) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (8,3,'Flash 4 Magic',36,'images/books/0735709491.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (9,1,'Web Development with PHP 4.0',36,'images/books/0735709971.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (10,2,'Beginning ASP Databases',39.99,'images/books/1861002726.jpg',NULL,1) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (11,2,'Oracle8i Web Development',41.99,'images/books/0072122420.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (12,1,'Black Belt Web Programming Methods',27.96,'images/books/0879304979.gif',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (13,1,'Web Development with JSP',35.96,'images/books/1884777996.jpg',NULL,1) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (14,1,'Professional JSP J2EE Edition',47.99,'images/books/1861004656.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (15,1,'Mastering ColdFusion 4.5',39.99,'images/books/0782127738.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (16,1,'Teach Yourself ColdFusion in 21 Days',31.99,'images/books/0672317966.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (17,1,'ColdFusion Fast & Easy Development',19.99,'images/books/0761530169.jpg',NULL,1) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (18,1,'Beginning Active Server Pages 3.0',31.99,'images/books/1861003382.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (19,3,'Web Design in a Nutshell',23.96,'images/books/1565925157.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (20,3,'HTML 4',15.99,'images/books/0201354934.jpg',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (21,3,'1001 Web Site Tips and Tricks',39.95,'images/books/1884133193.gif',NULL,0) ;
INSERT INTO store_products (product_id,category_id,product_name,price,image_url,description,is_recommended) 
       VALUES (22,1,'C# - Programming',34.99,'images/books/1861004877.jpg',NULL,0) ;


INSERT INTO store_editorial_products (article_id,editorial_cat_id,article_title,article_desc,product_id) 
  VALUES (1,1,'A Sharp Combination','To get inside C#, Microsoft\'s new OO programming language, use A Preview of C# as a guide. It offers a preview of Visual Studio.NET and an overview of the .NET framework, and demonstrates how C# is integrated with ASP+, ADO+, and COM+ in .NET applications. You\'ll get examples of C# in action, too.',22);
INSERT INTO store_editorial_products (article_id,editorial_cat_id,article_title,article_desc,product_id) 
  VALUES (2,2,'1001 Web Site Construction Tips and Tricks','The lowest price 39.95',21);
INSERT INTO store_editorial_products (article_id,editorial_cat_id,article_title,article_desc,product_id) 
  VALUES (3,3,'Flash 4 Magic','If you\'re right in the middle of learning (or just jumping into) Flash to create and manipulate animations, music tracks, sound effects, and interface design, try the Flash 4 Magic. Inside are tutorials, graphic presentations, and a CD.',8);
INSERT INTO store_editorial_products (article_id,editorial_cat_id,article_title,article_desc,product_id) 
  VALUES (4,4,'<b><font color=\"brown\">Free Shipping on orders over $40</font></b>','For limited time only, until next Sunday, you can enjoy free shipping. Simply order more then $40 worth of books and shipping\'s on us.',NULL);


INSERT INTO store_statuses (status_id,status_name) VALUES (1,'Purchased');
INSERT INTO store_statuses (status_id,status_name) VALUES (2,'Shipped'); 
INSERT INTO store_statuses (status_id,status_name) VALUES (3,'Canceled'); 


#INSERT INTO store_orders (order_id,user_id,order_date,order_status_id,total) 
#  VALUES (1,2,'2003-02-25 19:18:42',1,39.99);
#INSERT INTO store_orders (order_id,user_id,order_date,order_status_id,total) 
#  VALUES (2,2,'2003-02-25 22:31:00',1,35.96);
#INSERT INTO store_orders (order_id,user_id,order_date,order_status_id,total) 
#  VALUES (3,2,'2003-03-06 16:10:03',1,55.92);
#INSERT INTO store_orders (order_id,user_id,order_date,order_status_id,total) 
#  VALUES (4,2,'2003-03-06 16:11:07',1,74.97);
#INSERT INTO store_orders (order_id,user_id,order_date,order_status_id,total) 
#  VALUES (5,2,'2003-03-06 16:13:42',2,34.99);


#INSERT INTO store_order_items (order_item_id,order_id,product_id,quantity,price) 
#  VALUES (1,1,10,1,39.99);
#INSERT INTO store_order_items (order_item_id,order_id,product_id,quantity,price) 
#  VALUES (2,2,13,1,35.96);
#INSERT INTO store_order_items (order_item_id,order_id,product_id,quantity,price) 
#  VALUES (3,3,7,1,27.96);
#INSERT INTO store_order_items (order_item_id,order_id,product_id,quantity,price) 
#  VALUES (4,3,7,1,27.96);
#INSERT INTO store_order_items (order_item_id,order_id,product_id,quantity,price) 
#  VALUES (5,4,17,2,19.99);
#INSERT INTO store_order_items (order_item_id,order_id,product_id,quantity,price) 
#  VALUES (6,4,22,1,34.99);
#INSERT INTO store_order_items (order_item_id,order_id,product_id,quantity,price) 
#  VALUES (7,5,22,1,34.99);


#INSERT INTO store_shopping_carts (shopping_cart_id,date_add) VALUES (1,'2003-02-25 20:06:00');



