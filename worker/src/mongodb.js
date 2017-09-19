import config from './config';
import mongoose from 'mongoose';

const mongoUri = config.mongo.host;

mongoose.connect(mongoUri, {server: {socketOptions: {keepAlive: 1} } });
mongoose.connection.on('error', () => {
	throw new Error(`unable to connect to database: ${mongoUri}`);
});

if (config.MONGOOSE_DEBUG) {
  mongoose.set('debug', (collectionName, method, query, doc) => {
    debug(`${collectionName}.${method}`, util.inspect(query, false, 20), doc);
  });
}
