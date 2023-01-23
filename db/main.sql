CREATE TABLE Wallet (
  id SERIAL PRIMARY KEY,
  name VARCHAR(255),
  seed VARCHAR(255),
  coin VARCHAR(255)
);

CREATE TABLE Address (
  id SERIAL PRIMARY KEY,
  drv_path VARCHAR(255),
  addr VARCHAR(255),
  addr_type VARCHAR(255),
  prv_key VARCHAR(255),
  pub_key VARCHAR(255),
  wallet_id INTEGER REFERENCES Wallet(id)
);
