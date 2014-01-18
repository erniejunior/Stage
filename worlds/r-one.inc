define r-one_bumper bumper
(
	color "random"
	block
	(
		points 22
		point[0] [0.101627 -0.042095]
		point[1] [0.104616 -0.033992]
		point[2] [0.106961 -0.025679]
		point[3] [0.108646 -0.017208]
		point[4] [0.109661 -0.008631]
		point[5] [0.110000 0.000000]
		point[6] [0.109661 0.008631]
		point[7] [0.108646 0.017208]
		point[8] [0.106961 0.025679]
		point[9] [0.104616 0.033992]
		point[10] [0.101627 0.042095]
		point[11] [0.083149 0.034442]
		point[12] [0.085595 0.027812]
		point[13] [0.087513 0.021010]
		point[14] [0.088892 0.014079]
		point[15] [0.089723 0.007061]
		point[16] [0.090000 0.000000]
		point[17] [0.089723 -0.007061]
		point[18] [0.088892 -0.014079]
		point[19] [0.087513 -0.021010]
		point[20] [0.085595 -0.027812]
		point[21] [0.083149 -0.034442]
		z [0 0.05]
	)
	size [0.026850842074 0.0841903551203 0.05]
)
define r-one_sender ircomm
(
	fov 64
	size [0.011000 0.011000 0.050000]
	is_sender 1
	is_receiver 0
	color "red"
)
define r-one_receiver ircomm
(
	fov 64
	size [0.011000 0.011000 0.050000]
	is_sender 0
	is_receiver 1
	color "blue"
)
define r-one position
(
	drive "diff"
	obstacle_return 1
	size [0.18 0.18 0.05]
	stack_children 0
	accelerometer ( )
	block
	(
		points 80
		point[0] [0.090000 0.000000]
		point[1] [0.089723 0.007061]
		point[2] [0.088892 0.014079]
		point[3] [0.087513 0.021010]
		point[4] [0.085595 0.027812]
		point[5] [0.083149 0.034442]
		point[6] [0.080191 0.040859]
		point[7] [0.076738 0.047025]
		point[8] [0.072812 0.052901]
		point[9] [0.068437 0.058450]
		point[10] [0.063640 0.063640]
		point[11] [0.058450 0.068437]
		point[12] [0.052901 0.072812]
		point[13] [0.047025 0.076738]
		point[14] [0.040859 0.080191]
		point[15] [0.034442 0.083149]
		point[16] [0.027812 0.085595]
		point[17] [0.021010 0.087513]
		point[18] [0.014079 0.088892]
		point[19] [0.007061 0.089723]
		point[20] [0.000000 0.090000]
		point[21] [-0.007061 0.089723]
		point[22] [-0.014079 0.088892]
		point[23] [-0.021010 0.087513]
		point[24] [-0.027812 0.085595]
		point[25] [-0.034442 0.083149]
		point[26] [-0.040859 0.080191]
		point[27] [-0.047025 0.076738]
		point[28] [-0.052901 0.072812]
		point[29] [-0.058450 0.068437]
		point[30] [-0.063640 0.063640]
		point[31] [-0.068437 0.058450]
		point[32] [-0.072812 0.052901]
		point[33] [-0.076738 0.047025]
		point[34] [-0.080191 0.040859]
		point[35] [-0.083149 0.034442]
		point[36] [-0.085595 0.027812]
		point[37] [-0.087513 0.021010]
		point[38] [-0.088892 0.014079]
		point[39] [-0.089723 0.007061]
		point[40] [-0.090000 0.000000]
		point[41] [-0.089723 -0.007061]
		point[42] [-0.088892 -0.014079]
		point[43] [-0.087513 -0.021010]
		point[44] [-0.085595 -0.027812]
		point[45] [-0.083149 -0.034442]
		point[46] [-0.080191 -0.040859]
		point[47] [-0.076738 -0.047025]
		point[48] [-0.072812 -0.052901]
		point[49] [-0.068437 -0.058450]
		point[50] [-0.063640 -0.063640]
		point[51] [-0.058450 -0.068437]
		point[52] [-0.052901 -0.072812]
		point[53] [-0.047025 -0.076738]
		point[54] [-0.040859 -0.080191]
		point[55] [-0.034442 -0.083149]
		point[56] [-0.027812 -0.085595]
		point[57] [-0.021010 -0.087513]
		point[58] [-0.014079 -0.088892]
		point[59] [-0.007061 -0.089723]
		point[60] [-0.000000 -0.090000]
		point[61] [0.007061 -0.089723]
		point[62] [0.014079 -0.088892]
		point[63] [0.021010 -0.087513]
		point[64] [0.027812 -0.085595]
		point[65] [0.034442 -0.083149]
		point[66] [0.040859 -0.080191]
		point[67] [0.047025 -0.076738]
		point[68] [0.052901 -0.072812]
		point[69] [0.058450 -0.068437]
		point[70] [0.063640 -0.063640]
		point[71] [0.068437 -0.058450]
		point[72] [0.072812 -0.052901]
		point[73] [0.076738 -0.047025]
		point[74] [0.080191 -0.040859]
		point[75] [0.083149 -0.034442]
		point[76] [0.085595 -0.027812]
		point[77] [0.087513 -0.021010]
		point[78] [0.088892 -0.014079]
		point[79] [0.089723 -0.007061]
		z [0 1]
	)
	r-one_bumper
	(
		pose [0.096575 0.000000 0 0.000000]
	)
	r-one_bumper
	(
		pose [0.068289 0.068289 0 45.000000]
	)
	r-one_bumper
	(
		pose [0.000000 0.096575 0 90.000000]
	)
	r-one_bumper
	(
		pose [-0.068289 0.068289 0 135.000000]
	)
	r-one_bumper
	(
		pose [-0.096575 0.000000 0 180.000000]
	)
	r-one_bumper
	(
		pose [-0.068289 -0.068289 0 225.000000]
	)
	r-one_bumper
	(
		pose [-0.000000 -0.096575 0 270.000000]
	)
	r-one_bumper
	(
		pose [0.068289 -0.068289 0 315.000000]
	)
	r-one_sender
	(
		pose [0.110000 0.000000 0.025000 0.000000]
	)
	r-one_receiver
	(
		pose [0.101627 0.042095 0.025000 22.500000]
	)
	r-one_sender
	(
		pose [0.077782 0.077782 0.025000 45.000000]
	)
	r-one_receiver
	(
		pose [0.042095 0.101627 0.025000 67.500000]
	)
	r-one_sender
	(
		pose [0.000000 0.110000 0.025000 90.000000]
	)
	r-one_receiver
	(
		pose [-0.042095 0.101627 0.025000 112.500000]
	)
	r-one_sender
	(
		pose [-0.077782 0.077782 0.025000 135.000000]
	)
	r-one_receiver
	(
		pose [-0.101627 0.042095 0.025000 157.500000]
	)
	r-one_sender
	(
		pose [-0.110000 0.000000 0.025000 180.000000]
	)
	r-one_receiver
	(
		pose [-0.101627 -0.042095 0.025000 202.500000]
	)
	r-one_sender
	(
		pose [-0.077782 -0.077782 0.025000 225.000000]
	)
	r-one_receiver
	(
		pose [-0.042095 -0.101627 0.025000 247.500000]
	)
	r-one_sender
	(
		pose [-0.000000 -0.110000 0.025000 270.000000]
	)
	r-one_receiver
	(
		pose [0.042095 -0.101627 0.025000 292.500000]
	)
	r-one_sender
	(
		pose [0.077782 -0.077782 0.025000 315.000000]
	)
	r-one_receiver
	(
		pose [0.101627 -0.042095 0.025000 337.500000]
	)
)