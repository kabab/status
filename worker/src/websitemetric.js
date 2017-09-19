import webpage from 'webpage'

var url = "https://www.avito.ma"

const randomChoice = (tab) => tab[Math.floor(tab.length * Math.random())];

class WebsiteMetric {
	constructor(url) {
		this.page = webpage.create();
		this.page.settings.userAgent = randomChoice(WebsiteMetric.clients);
		this.reqAndRes = [];
		this.url = url;

		this.page.onResourceRequested = (req) => {
			this.reqAndRes.push(req);
		}

		this.page.onResourceReceived = (res) => {
			if (res.id == 1 && res.stage == "end") {
				// this.page.stop();
			}
			this.reqAndRes.push(res);
		}
	}

	start(cb) {
		this.page.open(this.url, (status) => {
			cb(status);
			this.page.release();
		});
	}

	calculate() {
		const endDate = this.reqAndRes.filter((e) => e.stage == "end" && e.id == 1)[0];
		const startDate = this.reqAndRes.filter((e) => e.method == "GET" && e.id == 1)[0];
		let diff = null;

		if (startDate && endDate) {
			const t1 = new Date(startDate.time);
			const t2 = new Date(endDate.time);
			this.delay = t2.getTime() - t1.getTime();
			this.time = new Date();
			return this.delay;
		} else {
			return 0;
		}
	}
}

WebsiteMetric.clients = [
	'Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/37.0.2062.120 Safari/537.36',
]

export { WebsiteMetric }

