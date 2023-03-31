Metsi control YAML structures defined as a simple schema.

Here follows a list of YAML element definitions, where

* `%X` denotes a previously defined element X.
* `%s` denotes any string value.
* `%i` denotes any string value convertible to the presented integer.
* `*` denotes optional repetition of the preceding element.
* `|` denotes exclusive or.
* anything else is verbatim in the YAML


`event_label`

    %s

`parameter_map`

    %s: %s
    *

`app_configuration`

    app_configuration:
      %parameter_map

`parameterized_event_label`

    %event_label:
      %parameter_map

`event_parameters`

    event_parameters:
      - %parameterized_event_label
      - *

`event_alias`

    %event_label:
      %event_label | %parameterized_event_label

`event_aliases`

    event_aliases:
      - %event_alias
      - *


`generator_type`

    alternatives  |  sequence

`generator`

    %generator_type:
      - %generator | %event_label | %parameterized_event_label
      - *

`generator_block`

    time_points: [%i, *]
    generators:
      - %generator
      - *

`simulation_events`

    simulation_events:
      - %generator_block
      - *

`control_yaml`

    %app_configuration
    %event_parameters
    %event_aliases
    %simulation_events
    

An example of a well-formed control YAML file

```
app_configuration:
  state_input_container: csv

event_parameters:
  grow:
    step: 5

event_aliases:
  grow_one_year:
    grow:
      step: 1

#starting state 0
simulation_events:
  - time_points: [2020]
    generators:
      - sequence:
          - grow_one_year
  - time_points: [2021]
    generators:
      - alternatives:
          - grow:
              step: 4
          - rest
  - time_points: [2025, 2030]
    generators:
      - sequence:
          - grow
```