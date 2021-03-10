FROM node:10.16.3 as base
WORKDIR /app
COPY . .
RUN npm install --loglevel verbose