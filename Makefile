start:
	docker compose up -d

build:
	docker compose up --build -d

clean:
	docker compose down
