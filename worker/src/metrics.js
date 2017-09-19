import request from 'request';
import Status from './status.model';

const sampleTimeMonitor = (url) => {
	let status = new Status({
		website: url
	});
	request
		.get({
			url,
			time: true,
		}, (err, response) => {
			if (err) {
				status.error = err.code;
			} else {
				status.time = response.elapsedTime;
				status.statusCode = response.statusCode;
				console.log('Request time in ms', response.elapsedTime, response.statusCode);
			}
			status.save();
		});
}

export { sampleTimeMonitor };
