import math
import typing

RESISTOR_ARG_T =  typing.Union[
    int,
    float,
    typing.Iterable[
        typing.Union[
            int,
            float
        ]
    ]
]


def dr_dri(rs, i):
    return 1.0 / pow(sum(1.0 / r for r in rs) * rs[i], 2)


def parallel_resistor_tolerance_calculator(
        r_vals: RESISTOR_ARG_T,
        r_sigmas: RESISTOR_ARG_T,
        n_rs: int = 1
):
    if type(r_vals) in (int, float):
        r_vals = [r_vals] * n_rs
        r_sigmas = [r_sigmas] * n_rs

    dr = math.sqrt(
        sum(pow(dr_dri(r_vals, i) * r_sigmas[i], 2) for i in range(len(r_vals)))
    )
    r = pow(sum(1.0 / r for r in r_vals), -1)
    return r, dr