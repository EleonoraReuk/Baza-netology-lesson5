create table clients (
	clients_id serial primary key,
	name varchar(100),
	surname varchar(100),
	email varchar(100)
);

create table phones (
	phones_id serial primary key,
	clients_id integer,
	phone text not null,

	foreign key (clients_id)
	references clients(clients_id)
);

select * from phones;
select * from clients;
