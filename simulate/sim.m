function res = sim()

	function res = pend(t, X)
		L = 1;

		g = 9.8;

		theta = X(1);
		w = X(2);

		dthetadt = w;
		dwdt = accel(t, theta, w);

		res = [dthetadt; dwdt];
	end

	function res = accel(t, theta, w)
		res = -9.8*sin(theta);
	end

	time = 0:1e-2:3;
	[T, Y] = ode45(@pend, time, [pi-.6, 0]);

	function draw_func(x1, y1)
		plot(x1, y1, 'r.', 'MarkerSize', 50); 
	end

	function animate_func(T, theta)
		minmax = [-1.5, 1.5, -1.5, 1.5];
		for i=1:length(T)
			clf;
			axis(minmax);
			hold on;
			grid on;

			x = sin(theta(i));
			y = -cos(theta(i));

			draw_func(x, y);
			drawnow;
		end
	end

	animate_func(T, Y);
end