import { WebsiteMetric } from './websitemetric';
import { args } from 'system';

if (args.length === 1) {
	console.error(`Need arguments usage: ${args[0]} url`);
	phantom.exit(1);
}

const url = args[1];

let metrics = new WebsiteMetric(url);

metrics.start((status) => {
	metrics.calculate();
	console.log(`status:${status}`);
	console.log(`time:${metrics.time.getTime()}`);
	console.log(`time:${metrics.delay}`);
	phantom.exit(0);
});