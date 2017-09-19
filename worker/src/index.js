import http from 'http';
import express from 'express';
import cors from 'cors';
import morgan from 'morgan';
import bodyParser from 'body-parser';
import mongoose from 'mongoose';
import config from './config'

let app = express();
app.server = http.createServer(app);

app.use(morgan(process.env.ENVIRONMENT || 'dev'));

app.use(cors({
	exposedHeaders: config.crosHeaders
}))

app.use(bodyParser.json({
	limit : config.bodyLimit
}));


app.server.listen(process.env.PORT || config.port, () => {
	console.log(`started on port ${app.server.address().port}`);
});

app.get('/free', (req, res) => {

})

// import { WebsiteMetric } from './websitemetric';

// let metrics = new WebsiteMetric("https://www.avito.ma");

// metrics.start(() => {
// 	metrics.calculate();
// 	phantom.exit(0);
// });
