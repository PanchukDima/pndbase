-- Synthetic, deterministic demo data for the reconstructed pndbase schema.
-- Run after database/deploy.sql. Re-running this file updates the same rows.
-- All people, identifiers, addresses and medical events below are fictional.

BEGIN;
SET client_encoding = 'UTF8';
SET DateStyle = 'ISO, DMY';
SET search_path = public, test, library, messager, reports_library, logs;

INSERT INTO library.departments (id, name) VALUES
    (10001, 'Тестовое амбулаторное отделение'),
    (10002, 'Тестовый дневной стационар')
ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name;

INSERT INTO test.area (id, name) VALUES
    (10001, 'Тестовый участок 1'),
    (10002, 'Тестовый участок 2')
ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name;

INSERT INTO test.staff (id, fname, name, mname, position, code, status, support_department) VALUES
    (10001, 'Тестов', 'Терапевт', 'Первый', 1, 'TEST-DOC-1', 0, '10001'),
    (10002, 'Демо', 'Психиатр', 'Второй', 1, 'TEST-DOC-2', 0, '10002'),
    (10003, 'Архивный', 'Сотрудник', 'Тестовый', 2, 'TEST-ARCHIVE', 1, '10001')
ON CONFLICT (id) DO UPDATE SET
    fname = EXCLUDED.fname, name = EXCLUDED.name, mname = EXCLUDED.mname,
    position = EXCLUDED.position, code = EXCLUDED.code, status = EXCLUDED.status,
    support_department = EXCLUDED.support_department;

INSERT INTO test.users (id, staff_id, user_login, password_login_md5) VALUES
    (10001, 10001, 'test_doctor', md5('test')),
    (10002, 10002, 'test_psychiatrist', md5('test'))
ON CONFLICT (id) DO UPDATE SET
    staff_id = EXCLUDED.staff_id, user_login = EXCLUDED.user_login,
    password_login_md5 = EXCLUDED.password_login_md5;

INSERT INTO test.rights (id, user_id, rights) VALUES
    (10001, 10001, repeat('1', 64)),
    (10002, 10002, repeat('1', 32) || repeat('0', 32))
ON CONFLICT (id) DO UPDATE SET user_id = EXCLUDED.user_id, rights = EXCLUDED.rights;

INSERT INTO test.servising_area (staff_id, area_id) VALUES
    (10001, 10001), (10001, 10002), (10002, 10002)
ON CONFLICT DO NOTHING;

INSERT INTO test.street (id, name, staff_add_id) VALUES
    (10001, 'Тестовая улица', 10001),
    (10002, 'Демонстрационный проспект', 10001),
    (10003, 'Учебный переулок', 10002)
ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name, staff_add_id = EXCLUDED.staff_add_id;

INSERT INTO library.contact_list (id, name) VALUES
    (10001, 'Тестовая поликлиника'),
    (10002, 'Учебный военкомат')
ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name;

INSERT INTO library.analizes (id, name) VALUES
    (10001, 'Тестовый общий анализ крови'),
    (10002, 'Тестовая биохимия')
ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name;

INSERT INTO library.preparate_list (id, international_name, description, dose) VALUES
    (10001, 'Тестопрепарат A', 'Синтетическая запись, не применять', '10 мг'),
    (10002, 'Тестопрепарат B', 'Синтетическая запись, не применять', '25 мг')
ON CONFLICT (id) DO UPDATE SET
    international_name = EXCLUDED.international_name,
    description = EXCLUDED.description, dose = EXCLUDED.dose;

INSERT INTO test.diagnos (id, name, code, staff_add_id) VALUES
    (10001, 'Тестовое тревожное расстройство', 'TEST-F41.1', 10001),
    (10002, 'Тестовое аффективное расстройство', 'TEST-F32.0', 10002),
    (10003, 'Тестовое расстройство адаптации', 'TEST-F43.2', 10001)
ON CONFLICT (id) DO UPDATE SET
    name = EXCLUDED.name, code = EXCLUDED.code, staff_add_id = EXCLUDED.staff_add_id;

-- Six deliberately varied patients: active, discharged, minor, military-age,
-- elderly and logically deleted. Fixed IDs make the script repeatable.
INSERT INTO test.patient
    (id, fname, name, mname, serial_passport, number_passport, status,
     staff_add_id, vidan, delete_row) VALUES
    (10001, 'Тестов', 'Алексей', 'Первый', 'TEST', '000001', 99, 10001, 'Тестовые данные', false),
    (10002, 'Демова', 'Мария', 'Вторая', 'TEST', '000002', 1, 10002, 'Тестовые данные', false),
    (10003, 'Учебный', 'Илья', 'Третий', 'TEST', '000003', 100, 10001, 'Тестовые данные', false),
    (10004, 'Примеров', 'Олег', 'Четвертый', 'TEST', '000004', 99, 10002, 'Тестовые данные', false),
    (10005, 'Сценарная', 'Анна', 'Пятая', 'TEST', '000005', 99, 10001, 'Тестовые данные', false),
    (10006, 'Удаленный', 'Пациент', 'Шестой', 'TEST', '000006', 99, 10001, 'Тестовые данные', true)
ON CONFLICT (id) DO UPDATE SET
    fname = EXCLUDED.fname, name = EXCLUDED.name, mname = EXCLUDED.mname,
    serial_passport = EXCLUDED.serial_passport, number_passport = EXCLUDED.number_passport,
    status = EXCLUDED.status, staff_add_id = EXCLUDED.staff_add_id,
    vidan = EXCLUDED.vidan, delete_row = EXCLUDED.delete_row;

INSERT INTO test.medcard
    (id, patient_id, sex, birthday, job_place, tutor, pt, group_lgot_preparat,
     area_id, serial_snils, district_id, group_inv_obsh, polis_serial,
     polis_number, department) VALUES
    (10001, 10001, true,  DATE '1986-04-12', 'Тестовая организация', false, false, '', 10001, 'TEST-001', 1, '', 'T', '000001', 10001),
    (10002, 10002, false, DATE '1993-09-25', 'Учебный центр', false, false, 'Тестовая льгота', 10002, 'TEST-002', 1, '3', 'T', '000002', 10001),
    (10003, 10003, true,  DATE '2012-01-15', 'Школа', true, false, '', 10001, 'TEST-003', 2, '', 'T', '000003', 10001),
    (10004, 10004, true,  DATE '2004-06-20', 'Тестовый колледж', false, true, '', 10002, 'TEST-004', 2, '', 'T', '000004', 10002),
    (10005, 10005, false, DATE '1951-11-03', 'Пенсионер', false, false, '', 10001, 'TEST-005', 1, '2', 'T', '000005', 10001),
    (10006, 10006, true,  DATE '1978-02-08', '', false, false, '', 10002, 'TEST-006', 1, '', 'T', '000006', 10001)
ON CONFLICT (id) DO UPDATE SET
    patient_id = EXCLUDED.patient_id, sex = EXCLUDED.sex, birthday = EXCLUDED.birthday,
    job_place = EXCLUDED.job_place, tutor = EXCLUDED.tutor, pt = EXCLUDED.pt,
    group_lgot_preparat = EXCLUDED.group_lgot_preparat, area_id = EXCLUDED.area_id,
    serial_snils = EXCLUDED.serial_snils, district_id = EXCLUDED.district_id,
    group_inv_obsh = EXCLUDED.group_inv_obsh, polis_serial = EXCLUDED.polis_serial,
    polis_number = EXCLUDED.polis_number, department = EXCLUDED.department;

INSERT INTO test.address_patient
    (id, medcard_id, street_id, building, home, flat, telefon, index_adrr) VALUES
    (10001, 10001, 10001, '', '10', '1', '+7-000-000-00-01', '000001'),
    (10002, 10002, 10002, 'А', '20', '2', '+7-000-000-00-02', '000002'),
    (10003, 10003, 10003, '', '3', '3', '+7-000-000-00-03', '000003'),
    (10004, 10004, 10002, '', '40', '4', '+7-000-000-00-04', '000004'),
    (10005, 10005, 10001, 'Б', '50', '5', '+7-000-000-00-05', '000005'),
    (10006, 10006, 10003, '', '60', '6', '+7-000-000-00-06', '000006')
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, street_id = EXCLUDED.street_id,
    building = EXCLUDED.building, home = EXCLUDED.home, flat = EXCLUDED.flat,
    telefon = EXCLUDED.telefon, index_adrr = EXCLUDED.index_adrr;

INSERT INTO test.uchet
    (id, medcard_id, on_uchet, off_uchet, why_removed_id, staff_add_id,
     direct, direct_from, delete_row) VALUES
    (10001, 10001, DATE '2024-01-10', NULL, -1, 10001, 1, 1, false),
    (10002, 10002, DATE '2022-03-14', DATE '2025-03-14', 1, 10002, 1, 1, false),
    (10003, 10003, DATE '2026-02-01', NULL, -1, 10001, 1, 1, false),
    (10004, 10004, DATE '2025-06-20', NULL, -1, 10002, 1, 1, false),
    (10005, 10005, DATE '2020-08-05', NULL, -1, 10001, 1, 1, false),
    (10006, 10006, DATE '2023-05-11', NULL, -1, 10001, 1, 1, true)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, on_uchet = EXCLUDED.on_uchet,
    off_uchet = EXCLUDED.off_uchet, why_removed_id = EXCLUDED.why_removed_id,
    staff_add_id = EXCLUDED.staff_add_id, direct = EXCLUDED.direct,
    direct_from = EXCLUDED.direct_from, delete_row = EXCLUDED.delete_row;

INSERT INTO test.diagnos_patient
    (id, medcard_id, diagnos_id, staff_add_id, fixing_diagnos_date,
     id_parent, delete_row, first) VALUES
    (10001, 10001, 10001, 10001, DATE '2024-01-10', NULL, false, true),
    (10002, 10002, 10002, 10002, DATE '2022-03-14', NULL, false, true),
    (10003, 10003, 10003, 10001, DATE '2026-02-01', NULL, false, true),
    (10004, 10004, 10001, 10002, DATE '2025-06-20', NULL, false, true),
    (10005, 10005, 10002, 10001, DATE '2020-08-05', NULL, false, true),
    (10006, 10001, 10003, 10002, DATE '2025-01-15', 10001, false, false)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, diagnos_id = EXCLUDED.diagnos_id,
    staff_add_id = EXCLUDED.staff_add_id,
    fixing_diagnos_date = EXCLUDED.fixing_diagnos_date,
    id_parent = EXCLUDED.id_parent, delete_row = EXCLUDED.delete_row, first = EXCLUDED.first;

INSERT INTO test.dynamic_view
    (id, medcard_id, on_date, delete_row, staff_add_id, group_disp_view,
     status, diagnos_id, stop_date, cause, staff_del_id) VALUES
    (10001, 10001, DATE '2024-01-10', false, 10001, 'Д-1', false, 10001, NULL, '', NULL),
    (10002, 10002, DATE '2022-03-14', false, 10002, 'Д-2', true, 10002, DATE '2025-03-14', 'Стойкое улучшение', 10002),
    (10003, 10004, DATE '2025-06-20', false, 10002, 'Д-1', false, 10004, NULL, '', NULL),
    (10004, 10005, DATE '2020-08-05', false, 10001, 'Д-3', false, 10005, NULL, '', NULL)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, on_date = EXCLUDED.on_date,
    delete_row = EXCLUDED.delete_row, staff_add_id = EXCLUDED.staff_add_id,
    group_disp_view = EXCLUDED.group_disp_view, status = EXCLUDED.status,
    diagnos_id = EXCLUDED.diagnos_id, stop_date = EXCLUDED.stop_date,
    cause = EXCLUDED.cause, staff_del_id = EXCLUDED.staff_del_id;

INSERT INTO test.visits_control
    (id, date_assigned, medcard_id, staff_add_id, delete_row, date_presence, cause) VALUES
    (10001, current_date - 7, 10001, 10001, false, current_date - 7, ''),
    (10002, current_date + 3, 10001, 10001, false, NULL, ''),
    (10003, current_date - 2, 10004, 10002, false, NULL, 'Не явился'),
    (10004, current_date + 14, 10005, 10001, false, NULL, '')
ON CONFLICT (id) DO UPDATE SET
    date_assigned = EXCLUDED.date_assigned, medcard_id = EXCLUDED.medcard_id,
    staff_add_id = EXCLUDED.staff_add_id, delete_row = EXCLUDED.delete_row,
    date_presence = EXCLUDED.date_presence, cause = EXCLUDED.cause;

INSERT INTO test.hospitalization
    (id, medcard_id, staff_add_id, who_wrote, date_up, date_down,
     delete_row, diagnose_id, doctor, first) VALUES
    (10001, 10002, 10002, 1, DATE '2024-02-01', DATE '2024-02-20', false, 10002, 10002, true),
    (10002, 10001, 10001, 2, current_date - 5, NULL, false, 10001, 10001, false)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, staff_add_id = EXCLUDED.staff_add_id,
    who_wrote = EXCLUDED.who_wrote, date_up = EXCLUDED.date_up,
    date_down = EXCLUDED.date_down, delete_row = EXCLUDED.delete_row,
    diagnose_id = EXCLUDED.diagnose_id, doctor = EXCLUDED.doctor, first = EXCLUDED.first;

INSERT INTO test.notwork_list
    (id, date_start, date_stop, medcard_id, count_day, staff_add_id,
     staff_edit_id, delete_row, diagnos_id, who_open, who_close, state,
     doctor_open, doctor_close) VALUES
    (10001, current_date - 10, current_date - 3, 10001, 8, 10001, 10001, false, 10001, 1, 1, true, 10001, 10001),
    (10002, current_date - 2, NULL, 10004, NULL, 10002, 10002, false, 10001, 2, NULL, false, 10002, 0)
ON CONFLICT (id) DO UPDATE SET
    date_start = EXCLUDED.date_start, date_stop = EXCLUDED.date_stop,
    medcard_id = EXCLUDED.medcard_id, count_day = EXCLUDED.count_day,
    staff_add_id = EXCLUDED.staff_add_id, staff_edit_id = EXCLUDED.staff_edit_id,
    delete_row = EXCLUDED.delete_row, diagnos_id = EXCLUDED.diagnos_id,
    who_open = EXCLUDED.who_open, who_close = EXCLUDED.who_close,
    state = EXCLUDED.state, doctor_open = EXCLUDED.doctor_open,
    doctor_close = EXCLUDED.doctor_close;

INSERT INTO test.comments (id, type, staff, comment, medcard_id, delete_row) VALUES
    (10001, 0, 10001, 'Тестовый комментарий к карте пациента.', 10001, false),
    (10002, 1, 10002, 'Демонстрационная запись для проверки истории.', 10002, false)
ON CONFLICT (id) DO UPDATE SET
    type = EXCLUDED.type, staff = EXCLUDED.staff, comment = EXCLUDED.comment,
    medcard_id = EXCLUDED.medcard_id, delete_row = EXCLUDED.delete_row;

INSERT INTO test.text_info_patients
    (id, medcard_id, type, date_creat, staff_add_id, text, delete_row, doc_id) VALUES
    (10001, 10001, 0, current_date - 30, 10001, 'Синтетический текст документа для тестирования.', false, 1),
    (10002, 10004, 1, current_date - 5, 10002, 'Синтетическая выписка без персональных данных.', false, 1)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, type = EXCLUDED.type,
    date_creat = EXCLUDED.date_creat, staff_add_id = EXCLUDED.staff_add_id,
    text = EXCLUDED.text, delete_row = EXCLUDED.delete_row, doc_id = EXCLUDED.doc_id;

INSERT INTO test.invalid_patient
    (id, medcard_id, staff_add_id, group_inv, priznan_trudosp, from_other,
     date_peresmotra, srok_ocherednogo_pereosvidet, bs, inv_child,
     delete_row, diagnos_id, first, place_work_id,
     stepen_utrachenoy_trudosposobnosti, other_recomendation, staff_edit_id) VALUES
    (10001, 10002, 10002, '3', true, false, DATE '2025-02-01', DATE '2027-02-01', false, false, false, 10002, true, 1, '30%', 'Тестовая рекомендация', 10002)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, staff_add_id = EXCLUDED.staff_add_id,
    group_inv = EXCLUDED.group_inv, priznan_trudosp = EXCLUDED.priznan_trudosp,
    from_other = EXCLUDED.from_other, date_peresmotra = EXCLUDED.date_peresmotra,
    srok_ocherednogo_pereosvidet = EXCLUDED.srok_ocherednogo_pereosvidet,
    bs = EXCLUDED.bs, inv_child = EXCLUDED.inv_child,
    delete_row = EXCLUDED.delete_row, diagnos_id = EXCLUDED.diagnos_id,
    first = EXCLUDED.first, place_work_id = EXCLUDED.place_work_id,
    stepen_utrachenoy_trudosposobnosti = EXCLUDED.stepen_utrachenoy_trudosposobnosti,
    other_recomendation = EXCLUDED.other_recomendation,
    staff_edit_id = EXCLUDED.staff_edit_id;

INSERT INTO test.suicide
    (id, datestart, tsuicide_id, staff_add_id, staff_edit_id, medcard_id, delete_row) VALUES
    (10001, DATE '2024-05-12', 0, 10002, 10002, 10002, false)
ON CONFLICT (id) DO UPDATE SET
    datestart = EXCLUDED.datestart, tsuicide_id = EXCLUDED.tsuicide_id,
    staff_add_id = EXCLUDED.staff_add_id, staff_edit_id = EXCLUDED.staff_edit_id,
    medcard_id = EXCLUDED.medcard_id, delete_row = EXCLUDED.delete_row;

INSERT INTO test.ood
    (id, medcard_id, date_ood, criminal_article, staff_add_id, staff_edit_id, delete_row) VALUES
    (10001, 10004, DATE '2025-07-01', 'Тестовая статья', 10002, 10002, false)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, date_ood = EXCLUDED.date_ood,
    criminal_article = EXCLUDED.criminal_article,
    staff_add_id = EXCLUDED.staff_add_id, staff_edit_id = EXCLUDED.staff_edit_id,
    delete_row = EXCLUDED.delete_row;

INSERT INTO test.day_stationar
    (id, medcard_id, staff_add_id, staff_edit_id, delete_row, day_in, day_out, this_close) VALUES
    (10001, 10001, 10001, 10001, false, current_date - 12, NULL, false),
    (10002, 10002, 10002, 10002, false, DATE '2024-01-10', DATE '2024-01-25', true)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, staff_add_id = EXCLUDED.staff_add_id,
    staff_edit_id = EXCLUDED.staff_edit_id, delete_row = EXCLUDED.delete_row,
    day_in = EXCLUDED.day_in, day_out = EXCLUDED.day_out, this_close = EXCLUDED.this_close;

INSERT INTO test.preparate
    (id, staff_add_id, staff_edit_id, deleted_row, medcard_id,
     date_first_appointment, date_vk, preparate_id) VALUES
    (10001, 10001, 10001, false, 10001, DATE '2025-01-15', DATE '2025-02-15', 10001),
    (10002, 10002, 10002, false, 10002, DATE '2024-03-01', NULL, 10002)
ON CONFLICT (id) DO UPDATE SET
    staff_add_id = EXCLUDED.staff_add_id, staff_edit_id = EXCLUDED.staff_edit_id,
    deleted_row = EXCLUDED.deleted_row, medcard_id = EXCLUDED.medcard_id,
    date_first_appointment = EXCLUDED.date_first_appointment,
    date_vk = EXCLUDED.date_vk, preparate_id = EXCLUDED.preparate_id;

INSERT INTO test.data_analizes
    (id, medcard_id, staff_add_id, staff_edit_id, staff_del_id,
     delete_row, date_appointment, type_analizes) VALUES
    (10001, 10001, 10001, 10001, 0, false, current_date + 5, 10001),
    (10002, 10002, 10002, 10002, 0, false, current_date - 20, 10002)
ON CONFLICT (id) DO UPDATE SET
    medcard_id = EXCLUDED.medcard_id, staff_add_id = EXCLUDED.staff_add_id,
    staff_edit_id = EXCLUDED.staff_edit_id, staff_del_id = EXCLUDED.staff_del_id,
    delete_row = EXCLUDED.delete_row, date_appointment = EXCLUDED.date_appointment,
    type_analizes = EXCLUDED.type_analizes;

INSERT INTO test.journal_rvk
    (id, date_creat_record, doctor, medcard_id, street_id, house, building,
     flat, diagnos_id, result, who_signed_predsed, who_signed_zav_otdel,
     who_signed_doctor, when_signed, name_responsible, staff_add_id,
     staff_edit_id, delete_row) VALUES
    (10001, current_date - 10, 10001, 10001, 10001, '10', '', '1', 10001,
     'Тестовое решение комиссии', 10002, 10002, 10001, current_date - 9,
     'Тестовый ответственный', 10001, 10001, false)
ON CONFLICT (id) DO UPDATE SET
    date_creat_record = EXCLUDED.date_creat_record, doctor = EXCLUDED.doctor,
    medcard_id = EXCLUDED.medcard_id, street_id = EXCLUDED.street_id,
    house = EXCLUDED.house, building = EXCLUDED.building, flat = EXCLUDED.flat,
    diagnos_id = EXCLUDED.diagnos_id, result = EXCLUDED.result,
    who_signed_predsed = EXCLUDED.who_signed_predsed,
    who_signed_zav_otdel = EXCLUDED.who_signed_zav_otdel,
    who_signed_doctor = EXCLUDED.who_signed_doctor, when_signed = EXCLUDED.when_signed,
    name_responsible = EXCLUDED.name_responsible,
    staff_add_id = EXCLUDED.staff_add_id, staff_edit_id = EXCLUDED.staff_edit_id,
    delete_row = EXCLUDED.delete_row;

INSERT INTO test.journal_zapros
    (id, in_date_doc, in_number_doc, who_send, fname_p, name_p, oname_p,
     birthday_p, street_id, building_p, house_p, flat_p, area_id, diagnos_id,
     doctor_id, who_signed, date_signed, out_date_doc, out_number_doc,
     staff_id_respons, delete_row) VALUES
    (10001, current_date - 15, 100001, 10001, 'Запросов', 'Тест', 'Первый',
     DATE '1980-01-01', 10001, 1, 10, 1, 10001, 10001, 10001, 1,
     current_date - 14, current_date - 13, 200001, 10001, false),
    (10002, current_date - 3, 100002, 10002, 'Примерова', 'Демо', 'Вторая',
     DATE '1990-02-02', 10002, 0, 20, 2, 10002, 10002, 10002, NULL,
     NULL, NULL, NULL, 10002, false)
ON CONFLICT (id) DO UPDATE SET
    in_date_doc = EXCLUDED.in_date_doc, in_number_doc = EXCLUDED.in_number_doc,
    who_send = EXCLUDED.who_send, fname_p = EXCLUDED.fname_p,
    name_p = EXCLUDED.name_p, oname_p = EXCLUDED.oname_p,
    birthday_p = EXCLUDED.birthday_p, street_id = EXCLUDED.street_id,
    building_p = EXCLUDED.building_p, house_p = EXCLUDED.house_p,
    flat_p = EXCLUDED.flat_p, area_id = EXCLUDED.area_id,
    diagnos_id = EXCLUDED.diagnos_id, doctor_id = EXCLUDED.doctor_id,
    who_signed = EXCLUDED.who_signed, date_signed = EXCLUDED.date_signed,
    out_date_doc = EXCLUDED.out_date_doc, out_number_doc = EXCLUDED.out_number_doc,
    staff_id_respons = EXCLUDED.staff_id_respons, delete_row = EXCLUDED.delete_row;

INSERT INTO test.session
    (id, ip_adres, date_time_start, date_time_stop, staff, state) VALUES
    (10001, '127.0.0.1', current_timestamp - interval '2 hours', current_timestamp - interval '1 hour', 10001, 0),
    (10002, '127.0.0.1', current_timestamp - interval '30 minutes', NULL, 10002, 1)
ON CONFLICT (id) DO UPDATE SET
    ip_adres = EXCLUDED.ip_adres, date_time_start = EXCLUDED.date_time_start,
    date_time_stop = EXCLUDED.date_time_stop, staff = EXCLUDED.staff, state = EXCLUDED.state;

INSERT INTO test.tasks (id, text_task, status, session_id) VALUES
    (10001, 'Проверить карточку тестового пациента', 1, 10001),
    (10002, 'Проверить предстоящие посещения', 0, 10002)
ON CONFLICT (id) DO UPDATE SET
    text_task = EXCLUDED.text_task, status = EXCLUDED.status, session_id = EXCLUDED.session_id;

INSERT INTO test.logs (id, staff_add_id, date_add, text) VALUES
    (10001, 10001, current_date, 'Загружены синтетические тестовые данные')
ON CONFLICT (id) DO UPDATE SET
    staff_add_id = EXCLUDED.staff_add_id, date_add = EXCLUDED.date_add, text = EXCLUDED.text;

INSERT INTO logs.error (id, text) VALUES
    (10001, 'Тестовая запись журнала ошибок, реальной ошибки нет')
ON CONFLICT (id) DO UPDATE SET text = EXCLUDED.text;

INSERT INTO messager."group" (id, name, staff_id) VALUES
    (10001, 'Тестовая группа', ARRAY[10001, 10002]::bigint[])
ON CONFLICT (id) DO UPDATE SET name = EXCLUDED.name, staff_id = EXCLUDED.staff_id;

INSERT INTO messager.messages
    (id, staff_from, staff_to, message, date_add, read_at) VALUES
    (10001, 10001, 10002, 'Тестовое личное сообщение', current_timestamp - interval '20 minutes', current_timestamp - interval '10 minutes'),
    (10002, 10002, 10001, 'Непрочитанное тестовое сообщение', current_timestamp - interval '5 minutes', NULL)
ON CONFLICT (id) DO UPDATE SET
    staff_from = EXCLUDED.staff_from, staff_to = EXCLUDED.staff_to,
    message = EXCLUDED.message, date_add = EXCLUDED.date_add, read_at = EXCLUDED.read_at;

INSERT INTO messager.group_messages
    (id, group_id, staff_add_id, message, date_add) VALUES
    (10001, 10001, 10001, 'Тестовое сообщение в группе', current_timestamp - interval '15 minutes')
ON CONFLICT (id) DO UPDATE SET
    group_id = EXCLUDED.group_id, staff_add_id = EXCLUDED.staff_add_id,
    message = EXCLUDED.message, date_add = EXCLUDED.date_add;

-- Advance every identity sequence in the application schemas after fixed IDs.
DO $$
DECLARE
    r record;
    max_id bigint;
BEGIN
    FOR r IN
        SELECT table_schema, table_name
        FROM information_schema.columns
        WHERE column_name = 'id'
          AND is_identity = 'YES'
          AND table_schema IN ('test', 'library', 'messager', 'reports_library', 'logs')
    LOOP
        EXECUTE format('SELECT COALESCE(max(id), 0) FROM %I.%I', r.table_schema, r.table_name)
            INTO max_id;
        EXECUTE format(
            'SELECT setval(pg_get_serial_sequence(%L, %L), %s, true)',
            r.table_schema || '.' || quote_ident(r.table_name), 'id', GREATEST(max_id, 1)
        );
    END LOOP;
END
$$;

COMMIT;
