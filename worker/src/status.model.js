import mongoose from 'mongoose';

const StatusSchema = new mongoose.Schema({
	website: { type: String, required: true },
	created: { type: Date, default: Date.now },
	time: {type: Number},
	statusCode: Number,
	error: {type: String }
});

export default mongoose.model('Status', StatusSchema);