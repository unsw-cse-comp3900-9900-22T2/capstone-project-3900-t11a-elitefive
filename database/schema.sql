create type outcomeType as enum ('WIN', 'LOSS', 'DRAW');
create type gameType as enum ('CLASSIC', 'TRIPLES', 'POTHOLES');

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
	ranked boolean,
	end_time timestamptz default CURRENT_TIMESTAMP,
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

create table friendship (
	id serial,
	friend1 integer,
	friend2 integer,
	status integer default 1,
	action_by integer,
	tm timestamptz default CURRENT_TIMESTAMP,
	foreign key (friend1) references users (id),
	foreign key (friend2) references users (id),
	primary key (friend1, friend2)
);

-- Demo Data

-- Bots
INSERT INTO users (username) values 
('BOT1'),
('BOT2'),
('BOT3');

-- Our passwords are all admin.
insert into users (username, email, password_hash) values 
('David', 'david@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'),
('JackyJ', 'jackyj@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'),
('JackyX', 'jackyx@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'),
('Yirong', 'yirong@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'),
('Sage', 'sage@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918');

-- We are all friends.
insert into friendship (friend1, friend2) values 
(4, 5), (4, 6), (4, 7), (4, 8),
(5, 6), (5, 7), (5, 8),
(6, 7), (6, 8),
(7, 8);

-- Password = meowth
insert into users (username, email, password_hash) values
('Jesse', 'jesse@teamrocket.com', '619227d5cf63bffd286a6529f58fb3e679169230eb7b0151871b8f6583f24bc6'),
('James', 'james@teamrocket.com', '619227d5cf63bffd286a6529f58fb3e679169230eb7b0151871b8f6583f24bc6');

insert into friendship (friend1, friend2) values (9, 10);
