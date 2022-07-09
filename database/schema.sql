create type outcomeType as enum ('WIN', 'LOSS', 'DRAW');
create type gameType as enum ('CLASSIC', 'POTHOLES');

create table users (
	id serial, 
	username text, 
	email text unique,
	password_hash text,
	primary key (id)	
);

create table matches (
	id serial,
	game gameType, 
	start_time timestamp,
	replay text,
	primary key (id)
);

create table outcomes (
	player integer,
	match integer,
	end_elo integer,
	outcome outcomeType,
	foreign key (player) references users (id),
	foreign key (match) references matches (id),
	primary key (player, match)
);

create table snapshots (
	match integer,
	move_num integer,
	boardstate bigint,
	foreign key (match) references matches(id),
	primary key (match, move_num)
);

create table friends (
	friend1 integer,
	friend2 integer,
	foreign key (friend1) references users (id),
	foreign key (friend2) references users (id),
	primary key (friend1, friend2)
);

INSERT INTO users (username) values ('BOT1'), ('BOT2');
