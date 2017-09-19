export default {
	port: 8080,
	bodyLimit: '100kb',
	corsHeaders: [ 'Link' ],
	mongo: {
		host: 'mongodb://mongo/status'
	},
	port: 3000,
	rabbitmq: {
		host: 'amqp://user:password@rabbitmq'
	}
}