create type outcomeType as enum ('WIN', 'LOSE', 'DRAW');
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



-- demo data


-- our passwords r all admin 
insert into users values (1, 'David', 'david@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918');
insert into users values (2, 'JackyJ', 'jackyj@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918');
insert into users values (3, 'JackyX', 'jackyx@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918');
insert into users values (4, 'Yirong', 'yirong@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918');
insert into users values (5, 'Sage', 'sage@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918');

-- we are all friends
insert into friends values(1,2);
insert into friends values(1,3);
insert into friends values(1,4);
insert into friends values(1,5);
insert into friends values(2,3);
insert into friends values(2,4);
insert into friends values(2,5);
insert into friends values(3,4);
insert into friends values(3,5);
insert into friends values(4,5);

-- password = meowth
insert into users values (6, 'Jesse', 'jesse@teamrocket.com', '619227d5cf63bffd286a6529f58fb3e679169230eb7b0151871b8f6583f24bc6');
insert into users values (7, 'James', 'james@teamrocket.com', '619227d5cf63bffd286a6529f58fb3e679169230eb7b0151871b8f6583f24bc6');

insert into friends values(6,7);