create type outcomeType as enum ('WIN', 'LOSS', 'DRAW');
create type gameType as enum ('CLASSIC', 'TRIPLES', 'POTHOLES');

create table users (
	id serial, 
	username text, 
	email text unique,
	password_hash text,
	varified boolean default null,
	primary key (id)
);

create table varification_codes (
	userid integer,
	code text, 
	foreign key (userid) references users (id),
	primary key (userid)
);

create table matches (
	id serial,
	game gameType,
	ranked boolean,
	end_time timestamptz default CURRENT_TIMESTAMP,
	potholes text,
	replay text,
	svg_data text,
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
	primary key (friend1, friend2),
	check (friend1 < friend2)
);

create table friendreqs (
	from_user integer,
	to_user integer,
	foreign key (from_user) references users (id),
	foreign key (to_user) references users (id),
	primary key (from_user, to_user)
);

-- Demo Data

-- Our passwords are all admin.
insert into users (username, email, password_hash) values 
('David', 'david@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'),
('JackyJ', 'jackyj@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'),
('JackyX', 'jackyx@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'),
('Yirong', 'yirong@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918'),
('Sage', 'sage@unsw.edu.au', '8c6976e5b5410415bde908bd4dee15dfb167a9c873fc4bb8a81f6f2ab448a918');

-- Bots
INSERT INTO users (username) values 
('BOT1'),
('BOT2'),
('BOT3');

-- We are all friends.
insert into friends (friend1, friend2) values 
(1, 2), (1, 3), (1, 4), (1, 5),
(2, 3), (2, 4), (2, 5),
(3, 4), (3, 5),
(4, 5);

-- Password = meowth
insert into users (username, email, password_hash) values
('Jesse', 'jesse@teamrocket.com', '619227d5cf63bffd286a6529f58fb3e679169230eb7b0151871b8f6583f24bc6'),
('James', 'james@teamrocket.com', '619227d5cf63bffd286a6529f58fb3e679169230eb7b0151871b8f6583f24bc6');

insert into friends (friend1, friend2) values (9, 10);

-- Insert Matches.
insert into matches(game, ranked, potholes, replay, svg_data) values 
('CLASSIC', true, '', 'e5a1d8a4c4d4h5d1f3a2d7a3', ''),
('CLASSIC', true, '', 'd5a1g2d4e7a4g6a2d6a3', ''),
('CLASSIC', true, '', 'e5a1c3a4i4d1f2d3c4d4c6d2', ''),
('CLASSIC', true, '', 'e4a1e7a4g7d1e5e6a2d4c3b1b2c1', ''),
('CLASSIC', true, '', 'e4a1e7a4e5e6a3d1a2d4d6b1c5', ''),
('CLASSIC', false, '', 'a1a2a5b2c2e5c3d5c5c4b3d2e3d3d4g6e6d8b6d6d7b4a4b5c6f7e8', ''),
('CLASSIC', false, '', 'e5a1', ''),
('CLASSIC', false, '', 'e4a1e7', '');

-- Insert Outcomes.
insert into outcomes values 
(1, 1, 970, 'LOSS'),
(2, 1, 1030, 'WIN'),
(3, 2, 970, 'LOSS'),
(4, 2, 1030, 'WIN'),
(3, 3, 940, 'LOSS'),
(1, 3, 1000, 'WIN'),
(2, 4, 1000, 'LOSS'),
(4, 4, 1060, 'WIN'),
(5, 5, 970, 'LOSS'),
(1, 5, 1030, 'WIN'),
(1, 6, 1000, 'LOSS'),
(4, 6, 1090, 'WIN'),
(2, 7, 1030, 'WIN'),
(3, 7, 910, 'LOSS'),
(2, 8, 1000, 'LOSS'),
(4, 8, 1120, 'WIN');

-- Insert Snapshots
insert into snapshots(match, move_num, boardstate) values 
(1, 1, 1073741824),
(1, 2, 1),
(1, 3, 1107296256),
(1, 4, 9),
(1, 5, 1107312640),
(1, 6, 2097161),
(1, 7, 18014399616794624),
(1, 8, 2359305),
(1, 9, 18014537055748096),
(1, 10, 2359307),
(1, 11, 18014537072525312),
(1, 12, 2359311),
(2, 1, 4194304),
(2, 2, 1),
(2, 3, 17592190238720),
(2, 4, 2097153),
(2, 5, 17596485206016),
(2, 6, 2097161),
(2, 7, 299071461916672),
(2, 8, 2097163),
(2, 9, 299071470305280),
(2, 10, 2097167),
(3, 1, 1073741824),
(3, 2, 1),
(3, 3, 1073750016),
(3, 4, 9),
(3, 5, 576460753377173504),
(3, 6, 262153),
(3, 7, 576460822096650240),
(3, 8, 1310729),
(3, 9, 576460822096666624),
(3, 10, 3407881),
(3, 11, 576460822096732160),
(3, 12, 3932169),
(4, 1, 536870912),
(4, 2, 1),
(4, 3, 4831838208),
(4, 4, 9),
(4, 5, 562954785259520),
(4, 6, 262153),
(4, 7, 562955859001344),
(4, 8, 2147745801),
(4, 9, 562955859001346),
(4, 10, 2149842953),
(4, 11, 562955859009538),
(4, 12, 2149842985),
(4, 13, 562955859009602),
(4, 14, 2149845033),
(5, 1, 536870912),
(5, 2, 1),
(5, 3, 4831838208),
(5, 4, 9),
(5, 5, 5905580032),
(5, 6, 2147483657),
(5, 7, 5905580036),
(5, 8, 2147745801),
(5, 9, 5905580038),
(5, 10, 2149842953),
(5, 11, 5913968646),
(5, 12, 2149842985),
(5, 13, 5914001414),
(6, 1, 1),
(6, 2, 2),
(6, 3, 17),
(6, 4, 66),
(6, 5, 4113),
(6, 6, 1073741890),
(6, 7, 12305),
(6, 8, 1077936194),
(6, 9, 45073),
(6, 10, 1077952578),
(6, 11, 45201),
(6, 12, 1078476866),
(6, 13, 268480657),
(6, 14, 1079525442),
(6, 15, 270577809),
(6, 16, 281476056236098),
(6, 17, 2418061457),
(6, 18, 281476089790530),
(6, 19, 2418062481),
(6, 20, 281476098179138),
(6, 21, 2434839697),
(6, 22, 281476098179394),
(6, 23, 2434839705),
(6, 24, 281476098179906),
(6, 25, 2434905241),
(6, 26, 283675121435458),
(6, 27, 11024839833),
(7, 1, 1073741824),
(7, 2, 1),
(8, 1, 536870912),
(8, 2, 1),
(8, 3, 4831838208);