create type identifier_type as enum ('GoodReads', 'ISBN 13', 'ISBN 10', 'LibraryAnything', 'OpenLibrary');

create table authors (
	author_id serial primary key,
	name character varying(255) not null,
	url character varying(255) not null
);

create table books (
	book_id serial primary key,
	title character varying(255) not null,
	notes character varying(255) not null,
	number_of_pages int not null,
	publish_date character varying(255) not null,
	url character varying(255) not null
);

create table book_authors (
	book_id int not null references books(book_id),
	author_id int not null references authors(author_id),
	primary key(book_id, author_id)
);

create table identifiers (
	identifier_id serial primary key,
	book_id int not null references books(book_id),
	id_type identifier_type,
	id_value character varying(255)
);
