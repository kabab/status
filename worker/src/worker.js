import amqp from 'amqplib/callback_api';
import config from './config';
import { sampleTimeMonitor } from './metrics'
import {} from './mongodb'

amqp.connect(config.rabbitmq.host, (err, conn) => {
	conn.createChannel((err, ch) => {
		let q = 'websites';
		ch.assertQueue(q, {durable: false});
		ch.consume(q, function(msg) {
			let url = msg.content.toString('utf8');
			sampleTimeMonitor(url);
			ch.ack(msg);
		});
	});
});
