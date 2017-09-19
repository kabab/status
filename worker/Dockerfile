FROM node:boron

WORKDIR /usr/src/app

COPY . .
RUN npm install
RUN npm run build
CMD ["npm", "start"]